#include "../libs/HElib/EncryptedArray.h"
#include <fstream>

/*
AntennaEncryptor
Object that handles the encryption of antennas.

Initializes pk, context and antenna hash from files.
Encrypts an antenna identifier.
Stores the encryption in a file.
*/

#define CTXT_FILE "result_cipher"
#define CONTEXT_FILE "context"
#define PUBLIC_KEY_FILE "key.pub"
#define DEBUG 1

class AntennaEncryptor{
  
  FHEcontext context;
  FHEPubKey public_key;
  ZZX G;
  EncryptedArray encrypted_array;
  std::map< string, string > antenna_hash;

FHEcontext InitializeContext(){
  fstream context_file;
  context_file.open(CONTEXT_FILE, fstream::in);
  unsigned long m1, p1, r1;
  readContextBase(context_file, m1, p1, r1);
  FHEcontext context_(m1, p1, r1);
  context_file >> context_;
  context_file.close();
  
  return context_;
}

FHEPubKey InitializePk(){
  fstream pk_file;
  pk_file.open (PUBLIC_KEY_FILE);
  FHEPubKey p_key(this->context);
  pk_file >> p_key;
  pk_file.close();

  return p_key;
}
EncryptedArray InitializeEncryptedArray(){
  this->G = this->context.alMod.getFactorsOverZZ()[0];
  EncryptedArray encrypted_array_(this->context, this->G);
  return encrypted_array_;
}
void InitializeHash(int mnc_){
  string hash_mnc[3] = {"","a_1_hash","a_2_hash" } ;

  std::ifstream hash_file(hash_mnc[mnc_].c_str());
  string mnc, lac, cid, a,z,pz, key;
  while (hash_file >> mnc >> lac >> cid >> a >> z >> pz)
  {
    key = lac + " " + cid;
    this->antenna_hash[key] = a + " " + z + " " + pz;
  }
}

vector<long> BuildVector(int code, int encrypted_array_size){
  //code starts in 1 / vector starts in 0. code<= encrypted_array_size
  vector<long> code_vector;
  for(int i = 0 ; i < code-1; i++)
    code_vector.push_back(0);
  code_vector.push_back(1);

  for(int i = code ; i < encrypted_array_size; i++)
    code_vector.push_back(0);

  return code_vector;
}

public:
  AntennaEncryptor(int mnc):
    context(InitializeContext()), public_key(InitializePk()), encrypted_array(InitializeEncryptedArray())
    {
      InitializeHash(mnc);
    }
  
  FHEcontext get_context(){
    return context;
  }
  FHEPubKey get_public_key(){
    return public_key;
  }
  EncryptedArray get_encrypted_array(){
    return encrypted_array;
  }
  std::map< string, string > get_antenna_hash(){
    return antenna_hash;
  }
  int get_antenna_code(int lac, int cid){
  
    int a_code, z_code, pz_code;
    stringstream lac_cid;
    lac_cid << lac << " " << cid;
    string in_code = lac_cid.str();
    string out_code = antenna_hash[in_code];
    stringstream ss_lac_cid(out_code);
    
    while(ss_lac_cid >> a_code >> z_code >> pz_code) {
      return a_code;
    }
    return 0;
      
  }
  int get_zone_code(int lac, int cid){

    int a_code, z_code, pz_code;
    stringstream lac_cid;
    lac_cid << lac << " " << cid;
    string in_code = lac_cid.str();
    string out_code = antenna_hash[in_code];
    stringstream ss_lac_cid(out_code);
    
    while(ss_lac_cid >> a_code >> z_code >> pz_code) {
      return z_code;
    }
    return 0;
  }
  int get_pzone_code(int lac, int cid){

    int a_code, z_code, pz_code;
    stringstream lac_cid;
    lac_cid << lac << " " << cid;
    string in_code = lac_cid.str();
    string out_code = antenna_hash[in_code];
    stringstream ss_lac_cid(out_code);
    
    while(ss_lac_cid >> a_code >> z_code >> pz_code) {
      return pz_code;
    }
    return 0;
  }
  
  Ctxt EncryptAntenna(int lac, int cid){
    if (DEBUG > 0) cerr << "encrypting: " << lac << " " << cid << endl;
    
    int antenna_code = get_antenna_code(lac,cid);
    int zone_code = get_zone_code(lac,cid);
    if (DEBUG > 0)
      cerr << antenna_code << " " << zone_code << endl;

    Ctxt antenna_cipher(this->public_key);
    PlaintextArray antenna_plaintext(this->encrypted_array);

    if(antenna_code == 0 && zone_code==0){
      //Nothing to encrypt
      throw 33;
    }

    std::vector<long> antenna_vector = BuildVector(antenna_code, this->encrypted_array.size());
    
    antenna_plaintext.encode(antenna_vector);
    this->encrypted_array.encrypt(antenna_cipher, this->public_key, antenna_plaintext);
    
    return antenna_cipher;

  }
  Ctxt EncryptZone(int lac, int cid){
    if (DEBUG > 1) cerr << "encrypting: " << lac << " " << cid << endl;
    
    int antenna_code = get_antenna_code(lac,cid);
    int zone_code = get_zone_code(lac,cid);
    if (DEBUG > 1)
      cerr << antenna_code << " " << zone_code << endl;

    Ctxt zone_cipher(this->public_key);
    PlaintextArray zone_plaintext(this->encrypted_array);

   
    if(antenna_code == 0 && zone_code==0){
      //Nothing to encrypt
      throw 33;
    }
    
    std::vector<long> zone_vector = BuildVector(zone_code, this->encrypted_array.size());
    
    zone_plaintext.encode(zone_vector);
    this->encrypted_array.encrypt(zone_cipher, this->public_key, zone_plaintext);
    
    return zone_cipher;

  }
  Ctxt CtxtFromString(string cipher_string){
    Ctxt cipher(public_key);
    stringstream ss_cipher;
    ss_cipher << cipher_string;
    ss_cipher >> cipher;
    return cipher; 
  }
  void CtxtToFile(Ctxt cipher){
    ofstream cipher_file;

    cipher_file.open(CTXT_FILE);
    cipher_file << cipher ;
    cipher_file.close();

  }
};

