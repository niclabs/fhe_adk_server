#include <zmq.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <unistd.h>

/*
querier.cpp
Interphase for "researchers" to ask for the number of records that correspond
to the given antenna (count function on encrypted data).

An antenna is defined by its identifiers: mnc, lac, cid.
The antenna identifier is sent to the PROCESSOR.
The result is received from the DECRYPTOR.

The communication with the PROCESSOR and DECRYPTOR are done through zeroMQ.
*/

#define DECRYPTOR_ADRESS "tcp://localhost:5557"
#define QUERIER_ADRESS "tcp://*:5555"
#define MAX_ANTENNA_ID_SIZE 20
#define IO_THREADS 1
#define SUCCESS 0



int main (){
  int mnc, lac, cid, result;
  // Prepare our context and socket
  zmq::context_t context (IO_THREADS);
  zmq::socket_t socket_to_processor (context, ZMQ_PUSH);
  socket_to_processor.bind (QUERIER_ADRESS);

  zmq::socket_t socket_from_decryptor (context, ZMQ_PULL);
  socket_from_decryptor.connect (DECRYPTOR_ADRESS);

  while(true){

    //ask mnc, lac and cid of the antenna you wish to count.
    std::cout << "Define the antenna to be counted." << std::endl;
    std::cout << "mnc? ";
    std::cin >> mnc;
    std::cout << "lac? ";
    std::cin >> lac;
    std::cout << "cid? ";
    std::cin >> cid;
    std::cout << std::endl;

    //Send message with the antenna identifiers to the processor
    zmq::message_t antenna_id_message(MAX_ANTENNA_ID_SIZE);
    snprintf ((char *) antenna_id_message.data(), MAX_ANTENNA_ID_SIZE ,
      "%d %d %d", mnc, lac, cid);

    std::cout << "Sending message mnc:" << mnc << " lac: " << lac << " cid: "<< cid << " to processor." << std::endl;
    std::cout << "Waiting for reply from decryptor..." << std::endl;

    socket_to_processor.send (antenna_id_message);

    // Get the result from decryptor.
    zmq::message_t result_m;
    socket_from_decryptor.recv (&result_m);
    std::istringstream iss(static_cast<char*>(result_m.data()));
    iss >> result ;
    std::cout << "Received result : " << result << std::endl;
  }
  

  return SUCCESS;
}

