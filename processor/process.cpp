#include <zmq.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include "Processor.h"

/*
process.cpp

Waits the querier for an antenna identifier.
Processes the number of records in the database that correspond to the antenna.
Sends to the encrypted result to the decryptor.
*/

#define QUERIER_ADRESS "tcp://localhost:5555"
#define PROCESSOR_ADRESS "tcp://*:5556"
#define CTXT_FILE "result_cipher"
#define MAX_ANTENNA_INDEX_SIZE 5
#define IO_THREADS 1
#define SUCCESS 0

int main ()
{
  int mnc, lac, cid, antenna_index;
  Processor processor;

  // Prepare our context and socket
  zmq::context_t context (IO_THREADS);
  zmq::socket_t socket_to_decryptor (context, ZMQ_PUSH);
  socket_to_decryptor.bind (PROCESSOR_ADRESS);

  zmq::socket_t socket_from_querier (context, ZMQ_PULL);
  socket_from_querier.connect (QUERIER_ADRESS);

  while(true){

    // receive antenna identifier from querier
    zmq::message_t antenna_id;
    socket_from_querier.recv (&antenna_id);
    

    std::istringstream iss(static_cast<char*>(antenna_id.data()));
    iss >> mnc >> lac >> cid ;
    std::cout << "Received antenna mnc: " << mnc << " lac: " << lac << " cid: "<< cid << " from querier." << std::endl;
    

    
    processor.Process(mnc, lac, cid, antenna_index);
    std::cout << antenna_index << std::endl;
     
    std::ifstream t;
    int length;
    t.open(CTXT_FILE);      // open input file
    t.seekg(0, std::ios::end);    // go to the end
    length = t.tellg();           // report location (this is the length)
    t.seekg(0, std::ios::beg);    // go back to the beginning
    char* buffer = new char[length];    // allocate memory for a buffer of appropriate dimension
    t.read(buffer, length);       // read the whole file into the buffer
    t.close(); 

    // send index and encrypted result to decryptor.
    zmq::message_t antenna_index_m (MAX_ANTENNA_INDEX_SIZE);
    sprintf ((char *) antenna_index_m.data(), "%d", antenna_index);
    std::cout << "Sending antenna index to decryptor " << std::endl;
    socket_to_decryptor.send (antenna_index_m);

    zmq::message_t enc_result (length+1);
    sprintf ((char *) enc_result.data(), "%s", buffer);
    std::cout << "Sending encrypted result to decryptor " << std::endl;
    socket_to_decryptor.send (enc_result);
    
  }
  

  return SUCCESS;
}

