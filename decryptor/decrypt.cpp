#include <zmq.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include "ResultDecryptor.h"

/*
decrypt.cpp

Waits the processor for antenna index and encrypted result to be decrypted.
Sends to the querier the decrypted result.

*/

#define DECRYPTOR_ADRESS "tcp://*:5557"
#define PROCESSOR_ADRESS "tcp://localhost:5556"
#define MAX_RESULT_SIZE 10
#define IO_THREADS 1
#define SUCCESS 0

int main ()
{
  int antenna_index, result;

  // Prepare our context and socket
  zmq::context_t context (IO_THREADS);
  zmq::socket_t socket_to_querier (context, ZMQ_PUSH);
  socket_to_querier.bind (DECRYPTOR_ADRESS);

  zmq::socket_t socket_from_processor (context, ZMQ_PULL);
  socket_from_processor.connect (PROCESSOR_ADRESS);

  while(true){

    //Receive antenna index from processor
    zmq::message_t antenna_index_m;
    socket_from_processor.recv (&antenna_index_m);
    std::istringstream iss(static_cast<char*>(antenna_index_m.data()));
    iss >> antenna_index ;
    std::cout << "Received antenna index from processor: " << antenna_index << std::endl;


    //Receive encrypted result from processor
    zmq::message_t enc_result;
    socket_from_processor.recv (&enc_result);
    std::cout << "Received encrypted result from processor" << std::endl;

    //Build cipher from message and decrypt
    ofstream cipher_file;
    cipher_file.open("resultado_");
    cipher_file <<  static_cast<char*>(enc_result.data());
    cipher_file.close();

    ResultDecryptor result_decryptor;
    Ctxt cipher = result_decryptor.BuildCipherFromFile();
    result = result_decryptor.DecryptResult(cipher, antenna_index);
    std::cout << result << std::endl;
    

    //Send decrypted result to querier
    zmq::message_t result_m (MAX_RESULT_SIZE);
    sprintf ((char *) result_m.data(), "%d", result);
    std::cout << "Sending result " << std::endl;
    socket_to_querier.send (result_m);
    
  }
  

  return SUCCESS;
}

