#include "Certificate.h"
#include <iomanip>

//unsigned char* Certificate::cert;
//unsigned char* Certificate::key;
//unsigned int	Certificate::certLen;
//unsigned int Certificate::keyLen;
X509* Certificate::cert;
EVP_PKEY* Certificate::key;
std::string Certificate::digest;
std::string Certificate::certS;
std::string Certificate::keyS;

/* Generates a 2048-bit RSA key. */
#include <openssl/sha.h>
std::string sha256(const std::string str)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}


EVP_PKEY* Certificate::generate_key()
{
    /* Allocate memory for the EVP_PKEY structure. */
    EVP_PKEY* pkey = EVP_PKEY_new();
    if (!pkey)
    {
        std::cerr << "Unable to create EVP_PKEY structure." << std::endl;
        return NULL;
    }

    /* Generate the RSA key and assign it to pkey. */
    RSA* rsa = RSA_generate_key(2048, RSA_F4, NULL, NULL);
    if (!EVP_PKEY_assign_RSA(pkey, rsa))
    {
        std::cerr << "Unable to generate 2048-bit RSA key." << std::endl;
        EVP_PKEY_free(pkey);
        return NULL;
    }

    /* The key has been generated, return it. */
    return pkey;
}

/* Generates a self-signed x509 certificate. */
X509* Certificate::generate_x509(EVP_PKEY* pkey)
{
    /* Allocate memory for the X509 structure. */
    X509* x509 = X509_new();
    if (!x509)
    {
        std::cerr << "Unable to create X509 structure." << std::endl;
        return NULL;
    }

    /* Set the serial number. */
    ASN1_INTEGER_set(X509_get_serialNumber(x509), 1);

    /* This certificate is valid from now until exactly one year from now. */
    X509_gmtime_adj(X509_get_notBefore(x509), 0);
    X509_gmtime_adj(X509_get_notAfter(x509), 31536000L);

    /* Set the public key for our certificate. */
    X509_set_pubkey(x509, pkey);

    /* We want to copy the subject name to the issuer name. */
    X509_NAME* name = X509_get_subject_name(x509);

    /* Set the country code and common name. */
    X509_NAME_add_entry_by_txt(name, "C", MBSTRING_ASC, (unsigned char*)"CA", -1, -1, 0);
    X509_NAME_add_entry_by_txt(name, "O", MBSTRING_ASC, (unsigned char*)"MyCompany", -1, -1, 0);
    X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, (unsigned char*)"", -1, -1, 0);

    /* Now set the issuer name. */
    X509_set_issuer_name(x509, name);

    /* Actually sign the certificate with our key. */
    if (!X509_sign(x509, pkey, EVP_sha1()))
    {
        std::cerr << "Error signing certificate." << std::endl;
        X509_free(x509);
        return NULL;
    }

    return x509;
}

bool Certificate::write_to_string(EVP_PKEY* pkey, X509* x509)
{
    cert = x509;
    key = pkey;
    ///* Open the PEM file for writing the key to disk. */
    //FILE* pkey_file = fopen("key.pem", "wb");
    //if (!pkey_file)
    //{
    //    std::cerr << "Unable to open \"key.pem\" for writing." << std::endl;
    //    return false;
    //}

    /* Write the key to disk. */
    BIO* keybio = BIO_new(BIO_s_mem());
    BIO* certbio = BIO_new(BIO_s_mem());

    bool ret = PEM_write_bio_PrivateKey(keybio, pkey, NULL, NULL, 0, NULL, NULL);

    //if (!ret)
    //{
    //    std::cerr << "Unable to write private key to disk." << std::endl;
    //    return false;
    //}

    ///* Open the PEM file for writing the certificate to disk. */
    ////FILE* x509_file = fopen("cert.pem", "wb");
    ////if (!x509_file)
    ////{
    ////    std::cerr << "Unable to open \"cert.pem\" for writing." << std::endl;
    ////    return false;
    ////}

    ///* Write the certificate to disk. */
    ret = PEM_write_bio_X509(certbio, x509);
    //

    //if (!ret)
    //{
    //    std::cerr << "Unable to write certificate to disk." << std::endl;
    //    return false;
    //}

    unsigned char* data = new unsigned char[5000];
    unsigned int len = 5000;

    int certLen = BIO_read(certbio, data, len);
    //cert = data;
    std::cout << "cert len is " << certLen   << std::endl;
   


    unsigned char* digestData = new unsigned char[1500];
    unsigned int digestLen = 0;

    X509_digest(x509, EVP_sha256(), digestData, &digestLen);
    certS = std::string((char*)data, certLen);


    int keyLen = BIO_read(keybio, data, len);
    keyS = std::string((char*)data, keyLen);




    std::ostringstream oss;
    for (int pos = 0; pos < digestLen - 1; pos++)
        oss << std::setfill('0') << std::setw(2) << std::uppercase <<std::hex << static_cast<int>(digestData[pos]) << ":";
    oss << std::setfill('0') << std::setw(2) << std::uppercase <<std::hex << static_cast<int>(digestData[digestLen-1]);

    digest = oss.str();
    std::cout << "Digest is " << digest << " " << std::endl;


    delete data;
    delete digestData;

    return true;
}

void Certificate::Init() {

    EVP_PKEY*  private_key = generate_key();
    X509* cerfificate_ = generate_x509(private_key);

    write_to_string(private_key, cerfificate_);

}
