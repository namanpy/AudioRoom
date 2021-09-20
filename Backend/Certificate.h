#pragma once

#include <cstdio>
#include <iostream>
#include <sstream>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/evp.h>

class Certificate {
public:
	EVP_PKEY* generate_key();
	X509* generate_x509(EVP_PKEY* pkey);
	bool write_to_string(EVP_PKEY* pkey, X509* x509);
	void Init();
	static std::string certS;
	static std::string keyS;
	//static unsigned int	certLen;
	//static unsigned int keyLen;
	//
	static X509* cert;
	static EVP_PKEY* key;

	static std::string digest;

};