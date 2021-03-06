//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 2004.
//
//  File:       bcrypt.h
//
//  Contents:   Cryptographic Primitive API Prototypes and Definitions
//
//----------------------------------------------------------------------------

#ifndef __BCRYPT_H__
#define __BCRYPT_H__

#include <winapifamily.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma region Desktop Family
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)

#ifndef WINAPI
#define WINAPI __stdcall
#endif

#ifndef _NTDEF_
	typedef _Return_type_success_(return >= 0) LONG NTSTATUS;
	typedef NTSTATUS *PNTSTATUS;
#endif

#ifndef BCRYPT_SUCCESS
#define BCRYPT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
#endif

#ifndef CONST
#define CONST const
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef OPTIONAL
#define OPTIONAL
#endif

	//
	//  Alignment macros
	//

	//
	// BCRYPT_OBJECT_ALIGNMENT must be a power of 2
	// We align all our internal data structures to 16 to
	// allow fast XMM memory accesses.
	// BCrypt callers do not need to take any alignment precautions.
	//
#define BCRYPT_OBJECT_ALIGNMENT    16

	//
	// BCRYPT_STRUCT_ALIGNMENT is an alignment macro that we no longer use.
	// It used to align declspec(align(4)) on x86 and declspec(align(8)) on x64/ia64 but
	// all structures that used it contained a pointer so they were already 4/8 aligned.
	//
#define BCRYPT_STRUCT_ALIGNMENT

	//
	// DeriveKey KDF Types
	//
#define BCRYPT_KDF_HASH                     L"HASH"
#define BCRYPT_KDF_HMAC                     L"HMAC"
#define BCRYPT_KDF_TLS_PRF                  L"TLS_PRF"
#define BCRYPT_KDF_SP80056A_CONCAT          L"SP800_56A_CONCAT"

	//
	// DeriveKey KDF BufferTypes
	//
	// For BCRYPT_KDF_HASH and BCRYPT_KDF_HMAC operations, there may be an arbitrary
	// number of KDF_SECRET_PREPEND and KDF_SECRET_APPEND buffertypes in the
	// parameter list.  The BufferTypes are processed in order of appearence
	// within the parameter list.
	//
#define KDF_HASH_ALGORITHM      0x0
#define KDF_SECRET_PREPEND      0x1
#define KDF_SECRET_APPEND       0x2
#define KDF_HMAC_KEY            0x3
#define KDF_TLS_PRF_LABEL       0x4
#define KDF_TLS_PRF_SEED        0x5
#define KDF_SECRET_HANDLE       0x6
#define KDF_TLS_PRF_PROTOCOL    0x7
#define KDF_ALGORITHMID         0x8
#define KDF_PARTYUINFO          0x9
#define KDF_PARTYVINFO          0xA
#define KDF_SUPPPUBINFO         0xB
#define KDF_SUPPPRIVINFO        0xC
#define KDF_LABEL               0xD
#define KDF_CONTEXT             0xE
#define KDF_SALT                0xF
#define KDF_ITERATION_COUNT     0x10
	//
	//  
	// Parameters for BCrypt(/NCrypt)KeyDerivation:
	// Generic parameters: 
	// KDF_GENERIC_PARAMETER and KDF_HASH_ALGORITHM are the generic parameters that can be passed for the following KDF algorithms:
	// BCRYPT/NCRYPT_SP800108_CTR_HMAC_ALGORITHM 
	//      KDF_GENERIC_PARAMETER = KDF_LABEL||0x00||KDF_CONTEXT 
	// BCRYPT/NCRYPT_SP80056A_CONCAT_ALGORITHM
	//      KDF_GENERIC_PARAMETER = KDF_ALGORITHMID || KDF_PARTYUINFO || KDF_PARTYVINFO {|| KDF_SUPPPUBINFO } {|| KDF_SUPPPRIVINFO }
	// BCRYPT/NCRYPT_PBKDF2_ALGORITHM
	//      KDF_GENERIC_PARAMETER = KDF_SALT
	// BCRYPT/NCRYPT_CAPI_KDF_ALGORITHM
	//      KDF_GENERIC_PARAMETER = Not used
	//
	// KDF specific parameters:
	// For BCRYPT/NCRYPT_SP800108_CTR_HMAC_ALGORITHM: 
	//      KDF_HASH_ALGORITHM, KDF_LABEL and KDF_CONTEXT are required
	// For BCRYPT/NCRYPT_SP80056A_CONCAT_ALGORITHM:
	//      KDF_HASH_ALGORITHM, KDF_ALGORITHMID, KDF_PARTYUINFO, KDF_PARTYVINFO are required
	//      KDF_SUPPPUBINFO, KDF_SUPPPRIVINFO are optional
	// For BCRYPT/NCRYPT_PBKDF2_ALGORITHM
	//      KDF_HASH_ALGORITHM is required
	//      KDF_ITERATION_COUNT, KDF_SALT are optional
	//      Iteration count, (if not specified) will default to 10,000
	// For BCRYPT/NCRYPT_CAPI_KDF_ALGORITHM
	//      KDF_HASH_ALGORITHM is required
	//
#define KDF_GENERIC_PARAMETER 0x11

#define KDF_KEYBITLENGTH      0x12


	//
	// DeriveKey Flags:
	//
	// KDF_USE_SECRET_AS_HMAC_KEY_FLAG causes the secret agreement to serve also
	// as the HMAC key.  If this flag is used, the KDF_HMAC_KEY parameter should
	// NOT be specified.
	//
#define KDF_USE_SECRET_AS_HMAC_KEY_FLAG 0x1

	//
	// BCrypt structs
	//

	typedef struct __BCRYPT_KEY_LENGTHS_STRUCT
	{
		ULONG   dwMinLength;
		ULONG   dwMaxLength;
		ULONG   dwIncrement;
	} BCRYPT_KEY_LENGTHS_STRUCT;

	typedef BCRYPT_KEY_LENGTHS_STRUCT BCRYPT_AUTH_TAG_LENGTHS_STRUCT;

	typedef struct _BCRYPT_OID
	{
		ULONG   cbOID;
		PUCHAR  pbOID;
	} BCRYPT_OID;

	typedef struct _BCRYPT_OID_LIST
	{
		ULONG       dwOIDCount;
		BCRYPT_OID  *pOIDs;
	} BCRYPT_OID_LIST;

	typedef struct _BCRYPT_PKCS1_PADDING_INFO
	{
		LPCWSTR pszAlgId;
	} BCRYPT_PKCS1_PADDING_INFO;

	typedef struct _BCRYPT_PSS_PADDING_INFO
	{
		LPCWSTR pszAlgId;
		ULONG   cbSalt;
	} BCRYPT_PSS_PADDING_INFO;

	typedef struct _BCRYPT_OAEP_PADDING_INFO
	{
		LPCWSTR pszAlgId;
		PUCHAR   pbLabel;
		ULONG   cbLabel;
	} BCRYPT_OAEP_PADDING_INFO;

#define BCRYPT_AUTHENTICATED_CIPHER_MODE_INFO_VERSION  1

#define BCRYPT_AUTH_MODE_CHAIN_CALLS_FLAG   0x00000001
#define BCRYPT_AUTH_MODE_IN_PROGRESS_FLAG   0x00000002

	typedef struct _BCRYPT_AUTHENTICATED_CIPHER_MODE_INFO
	{
		ULONG       cbSize;
		ULONG       dwInfoVersion;
		PUCHAR      pbNonce;
		ULONG       cbNonce;
		PUCHAR      pbAuthData;
		ULONG       cbAuthData;
		PUCHAR      pbTag;
		ULONG       cbTag;
		PUCHAR      pbMacContext;
		ULONG       cbMacContext;
		ULONG       cbAAD;
		ULONGLONG   cbData;
		ULONG       dwFlags;
	} BCRYPT_AUTHENTICATED_CIPHER_MODE_INFO, *PBCRYPT_AUTHENTICATED_CIPHER_MODE_INFO;

#define BCRYPT_INIT_AUTH_MODE_INFO(_AUTH_INFO_STRUCT_)    \
            RtlZeroMemory((&_AUTH_INFO_STRUCT_), sizeof(BCRYPT_AUTHENTICATED_CIPHER_MODE_INFO));  \
            (_AUTH_INFO_STRUCT_).cbSize = sizeof(BCRYPT_AUTHENTICATED_CIPHER_MODE_INFO);          \
            (_AUTH_INFO_STRUCT_).dwInfoVersion = BCRYPT_AUTHENTICATED_CIPHER_MODE_INFO_VERSION;

	//
	// BCrypt String Properties
	//

	// BCrypt(Import/Export)Key BLOB types
#define BCRYPT_OPAQUE_KEY_BLOB      L"OpaqueKeyBlob"
#define BCRYPT_KEY_DATA_BLOB        L"KeyDataBlob"
#define BCRYPT_AES_WRAP_KEY_BLOB    L"Rfc3565KeyWrapBlob"

	// BCryptGetProperty strings
#define BCRYPT_OBJECT_LENGTH        L"ObjectLength"
#define BCRYPT_ALGORITHM_NAME       L"AlgorithmName"
#define BCRYPT_PROVIDER_HANDLE      L"ProviderHandle"
#define BCRYPT_CHAINING_MODE        L"ChainingMode"
#define BCRYPT_BLOCK_LENGTH         L"BlockLength"
#define BCRYPT_KEY_LENGTH           L"KeyLength"
#define BCRYPT_KEY_OBJECT_LENGTH    L"KeyObjectLength"
#define BCRYPT_KEY_STRENGTH         L"KeyStrength"
#define BCRYPT_KEY_LENGTHS          L"KeyLengths"
#define BCRYPT_BLOCK_SIZE_LIST      L"BlockSizeList"
#define BCRYPT_EFFECTIVE_KEY_LENGTH L"EffectiveKeyLength"
#define BCRYPT_HASH_LENGTH          L"HashDigestLength"
#define BCRYPT_HASH_OID_LIST        L"HashOIDList"
#define BCRYPT_PADDING_SCHEMES      L"PaddingSchemes"
#define BCRYPT_SIGNATURE_LENGTH     L"SignatureLength"
#define BCRYPT_HASH_BLOCK_LENGTH    L"HashBlockLength"
#define BCRYPT_AUTH_TAG_LENGTH      L"AuthTagLength"
#define BCRYPT_PRIMITIVE_TYPE       L"PrimitiveType"
#define BCRYPT_IS_KEYED_HASH        L"IsKeyedHash"
#define BCRYPT_IS_REUSABLE_HASH     L"IsReusableHash"
#define BCRYPT_MESSAGE_BLOCK_LENGTH L"MessageBlockLength"

	// Additional BCryptGetProperty strings for the RNG Platform Crypto Provider
#define BCRYPT_PCP_PLATFORM_TYPE_PROPERTY    L"PCP_PLATFORM_TYPE"
#define BCRYPT_PCP_PROVIDER_VERSION_PROPERTY L"PCP_PROVIDER_VERSION"

#if (NTDDI_VERSION > NTDDI_WINBLUE || (NTDDI_VERSION == NTDDI_WINBLUE && defined(WINBLUE_KBSPRING14)))
#define BCRYPT_MULTI_OBJECT_LENGTH  L"MultiObjectLength"
#endif

	// BCryptSetProperty strings
#define BCRYPT_INITIALIZATION_VECTOR    L"IV"

	// Property Strings
#define BCRYPT_CHAIN_MODE_NA        L"ChainingModeN/A"
#define BCRYPT_CHAIN_MODE_CBC       L"ChainingModeCBC"
#define BCRYPT_CHAIN_MODE_ECB       L"ChainingModeECB"
#define BCRYPT_CHAIN_MODE_CFB       L"ChainingModeCFB"
#define BCRYPT_CHAIN_MODE_CCM       L"ChainingModeCCM"
#define BCRYPT_CHAIN_MODE_GCM       L"ChainingModeGCM"

	// Supported RSA Padding Types
#define BCRYPT_SUPPORTED_PAD_ROUTER     0x00000001
#define BCRYPT_SUPPORTED_PAD_PKCS1_ENC  0x00000002
#define BCRYPT_SUPPORTED_PAD_PKCS1_SIG  0x00000004
#define BCRYPT_SUPPORTED_PAD_OAEP       0x00000008
#define BCRYPT_SUPPORTED_PAD_PSS        0x00000010

	//
	//      BCrypt Flags
	//

#define BCRYPT_PROV_DISPATCH        0x00000001  // BCryptOpenAlgorithmProvider

#define BCRYPT_BLOCK_PADDING        0x00000001  // BCryptEncrypt/Decrypt

	// RSA padding schemes
#define BCRYPT_PAD_NONE             0x00000001
#define BCRYPT_PAD_PKCS1            0x00000002  // BCryptEncrypt/Decrypt BCryptSignHash/VerifySignature
#define BCRYPT_PAD_OAEP             0x00000004  // BCryptEncrypt/Decrypt
#define BCRYPT_PAD_PSS              0x00000008  // BCryptSignHash/VerifySignature
#if (NTDDI_VERSION >= NTDDI_WINBLUE)
#define BCRYPT_PAD_PKCS1_OPTIONAL_HASH_OID  0x00000010   //BCryptVerifySignature
#endif

#define BCRYPTBUFFER_VERSION        0

#endif /* WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) */
#pragma endregion

#pragma region Application Family
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP)

	typedef struct _BCryptBuffer {
		ULONG   cbBuffer;             // Length of buffer, in bytes
		ULONG   BufferType;           // Buffer type
		PVOID   pvBuffer;             // Pointer to buffer
	} BCryptBuffer, *PBCryptBuffer;

	typedef struct _BCryptBufferDesc {
		ULONG   ulVersion;            // Version number
		ULONG   cBuffers;             // Number of buffers
		PBCryptBuffer pBuffers;       // Pointer to array of buffers
	} BCryptBufferDesc, *PBCryptBufferDesc;

#endif /* WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP) */
#pragma endregion

#pragma region Desktop Family
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)

	//
	// Primitive handles
	//

	typedef PVOID BCRYPT_HANDLE;
	typedef PVOID BCRYPT_ALG_HANDLE;
	typedef PVOID BCRYPT_KEY_HANDLE;
	typedef PVOID BCRYPT_HASH_HANDLE;
	typedef PVOID BCRYPT_SECRET_HANDLE;

	//
	// Structures used to represent key blobs.
	//

#define BCRYPT_PUBLIC_KEY_BLOB       L"PUBLICBLOB"
#define BCRYPT_PRIVATE_KEY_BLOB      L"PRIVATEBLOB"

	typedef struct _BCRYPT_KEY_BLOB
	{
		ULONG   Magic;
	} BCRYPT_KEY_BLOB;

	// The BCRYPT_RSAPUBLIC_BLOB and BCRYPT_RSAPRIVATE_BLOB blob types are used
	// to transport plaintext RSA keys. These blob types will be supported by
	// all RSA primitive providers.
	// The BCRYPT_RSAPRIVATE_BLOB includes the following values:
	// Public Exponent
	// Modulus
	// Prime1
	// Prime2

#define BCRYPT_RSAPUBLIC_BLOB       L"RSAPUBLICBLOB"
#define BCRYPT_RSAPRIVATE_BLOB      L"RSAPRIVATEBLOB"
#define LEGACY_RSAPUBLIC_BLOB       L"CAPIPUBLICBLOB"
#define LEGACY_RSAPRIVATE_BLOB      L"CAPIPRIVATEBLOB"

#define BCRYPT_RSAPUBLIC_MAGIC      0x31415352  // RSA1
#define BCRYPT_RSAPRIVATE_MAGIC     0x32415352  // RSA2

	typedef struct _BCRYPT_RSAKEY_BLOB
	{
		ULONG   Magic;
		ULONG   BitLength;
		ULONG   cbPublicExp;
		ULONG   cbModulus;
		ULONG   cbPrime1;
		ULONG   cbPrime2;
	} BCRYPT_RSAKEY_BLOB;

	// The BCRYPT_RSAFULLPRIVATE_BLOB blob type is used to transport
	// plaintext private RSA keys.  It includes the following values:
	// Public Exponent
	// Modulus
	// Prime1
	// Prime2
	// Private Exponent mod (Prime1 - 1)
	// Private Exponent mod (Prime2 - 1)
	// Inverse of Prime2 mod Prime1
	// PrivateExponent
#define BCRYPT_RSAFULLPRIVATE_BLOB      L"RSAFULLPRIVATEBLOB"

#define BCRYPT_RSAFULLPRIVATE_MAGIC     0x33415352  // RSA3

	//Properties of secret agreement algorithms
#define BCRYPT_GLOBAL_PARAMETERS    L"SecretAgreementParam"
#define BCRYPT_PRIVATE_KEY          L"PrivKeyVal"

	// The BCRYPT_ECCPUBLIC_BLOB and BCRYPT_ECCPRIVATE_BLOB blob types are used
	// to transport plaintext ECC keys. These blob types will be supported by
	// all ECC primitive providers.
#define BCRYPT_ECCPUBLIC_BLOB           L"ECCPUBLICBLOB"
#define BCRYPT_ECCPRIVATE_BLOB          L"ECCPRIVATEBLOB"

#define BCRYPT_ECDH_PUBLIC_P256_MAGIC   0x314B4345  // ECK1
#define BCRYPT_ECDH_PRIVATE_P256_MAGIC  0x324B4345  // ECK2
#define BCRYPT_ECDH_PUBLIC_P384_MAGIC   0x334B4345  // ECK3
#define BCRYPT_ECDH_PRIVATE_P384_MAGIC  0x344B4345  // ECK4
#define BCRYPT_ECDH_PUBLIC_P521_MAGIC   0x354B4345  // ECK5
#define BCRYPT_ECDH_PRIVATE_P521_MAGIC  0x364B4345  // ECK6

#define BCRYPT_ECDSA_PUBLIC_P256_MAGIC  0x31534345  // ECS1
#define BCRYPT_ECDSA_PRIVATE_P256_MAGIC 0x32534345  // ECS2
#define BCRYPT_ECDSA_PUBLIC_P384_MAGIC  0x33534345  // ECS3
#define BCRYPT_ECDSA_PRIVATE_P384_MAGIC 0x34534345  // ECS4
#define BCRYPT_ECDSA_PUBLIC_P521_MAGIC  0x35534345  // ECS5
#define BCRYPT_ECDSA_PRIVATE_P521_MAGIC 0x36534345  // ECS6

	typedef struct _BCRYPT_ECCKEY_BLOB
	{
		ULONG   dwMagic;
		ULONG   cbKey;
	} BCRYPT_ECCKEY_BLOB, *PBCRYPT_ECCKEY_BLOB;

	// The BCRYPT_DH_PUBLIC_BLOB and BCRYPT_DH_PRIVATE_BLOB blob types are used
	// to transport plaintext DH keys. These blob types will be supported by
	// all DH primitive providers.
#define BCRYPT_DH_PUBLIC_BLOB           L"DHPUBLICBLOB"
#define BCRYPT_DH_PRIVATE_BLOB          L"DHPRIVATEBLOB"
#define LEGACY_DH_PUBLIC_BLOB           L"CAPIDHPUBLICBLOB"
#define LEGACY_DH_PRIVATE_BLOB          L"CAPIDHPRIVATEBLOB"

#define BCRYPT_DH_PUBLIC_MAGIC          0x42504844  // DHPB
#define BCRYPT_DH_PRIVATE_MAGIC         0x56504844  // DHPV

	typedef struct _BCRYPT_DH_KEY_BLOB
	{
		ULONG   dwMagic;
		ULONG   cbKey;
	} BCRYPT_DH_KEY_BLOB, *PBCRYPT_DH_KEY_BLOB;

	// Property Strings for DH
#define BCRYPT_DH_PARAMETERS            L"DHParameters"

#define BCRYPT_DH_PARAMETERS_MAGIC      0x4d504844  // DHPM

	typedef _Struct_size_bytes_(cbLength) struct _BCRYPT_DH_PARAMETER_HEADER
	{
		ULONG           cbLength;
		ULONG           dwMagic;
		ULONG           cbKeyLength;
	} BCRYPT_DH_PARAMETER_HEADER;


	// The BCRYPT_DSA_PUBLIC_BLOB and BCRYPT_DSA_PRIVATE_BLOB blob types are used
	// to transport plaintext DSA keys. These blob types will be supported by
	// all DSA primitive providers.
#define BCRYPT_DSA_PUBLIC_BLOB          L"DSAPUBLICBLOB"
#define BCRYPT_DSA_PRIVATE_BLOB         L"DSAPRIVATEBLOB"
#define LEGACY_DSA_PUBLIC_BLOB          L"CAPIDSAPUBLICBLOB"
#define LEGACY_DSA_PRIVATE_BLOB         L"CAPIDSAPRIVATEBLOB"
#define LEGACY_DSA_V2_PUBLIC_BLOB       L"V2CAPIDSAPUBLICBLOB"
#define LEGACY_DSA_V2_PRIVATE_BLOB      L"V2CAPIDSAPRIVATEBLOB"

#define BCRYPT_DSA_PUBLIC_MAGIC             0x42505344  // DSPB
#define BCRYPT_DSA_PRIVATE_MAGIC            0x56505344  // DSPV
#define BCRYPT_DSA_PUBLIC_MAGIC_V2          0x32425044  // DPB2
#define BCRYPT_DSA_PRIVATE_MAGIC_V2     0x32565044  // DPV2

	typedef struct _BCRYPT_DSA_KEY_BLOB
	{
		ULONG   dwMagic;
		ULONG   cbKey;
		UCHAR   Count[4];
		UCHAR   Seed[20];
		UCHAR   q[20];
	} BCRYPT_DSA_KEY_BLOB, *PBCRYPT_DSA_KEY_BLOB;

	typedef enum
	{
		DSA_HASH_ALGORITHM_SHA1,
		DSA_HASH_ALGORITHM_SHA256,
		DSA_HASH_ALGORITHM_SHA512
	}HASHALGORITHM_ENUM;

	typedef enum
	{
		DSA_FIPS186_2,
		DSA_FIPS186_3
	} DSAFIPSVERSION_ENUM;

	typedef struct _BCRYPT_DSA_KEY_BLOB_V2
	{
		ULONG                                   dwMagic;
		ULONG                                   cbKey;
		HASHALGORITHM_ENUM          hashAlgorithm;
		DSAFIPSVERSION_ENUM         standardVersion;
		ULONG                                   cbSeedLength;
		ULONG                                   cbGroupSize;
		UCHAR                                   Count[4];
	} BCRYPT_DSA_KEY_BLOB_V2, *PBCRYPT_DSA_KEY_BLOB_V2;

	typedef struct _BCRYPT_KEY_DATA_BLOB_HEADER
	{
		ULONG   dwMagic;
		ULONG   dwVersion;
		ULONG   cbKeyData;
	} BCRYPT_KEY_DATA_BLOB_HEADER, *PBCRYPT_KEY_DATA_BLOB_HEADER;

#define BCRYPT_KEY_DATA_BLOB_MAGIC       0x4d42444b //Key Data Blob Magic (KDBM)

#define BCRYPT_KEY_DATA_BLOB_VERSION1    0x1

	// Property Strings for DSA
#define BCRYPT_DSA_PARAMETERS       L"DSAParameters"

#define BCRYPT_DSA_PARAMETERS_MAGIC           0x4d505344  // DSPM

#define BCRYPT_DSA_PARAMETERS_MAGIC_V2     0x324d5044  // DPM2

	typedef struct _BCRYPT_DSA_PARAMETER_HEADER
	{
		ULONG           cbLength;
		ULONG           dwMagic;
		ULONG           cbKeyLength;
		UCHAR           Count[4];
		UCHAR           Seed[20];
		UCHAR           q[20];
	} BCRYPT_DSA_PARAMETER_HEADER;

	typedef struct _BCRYPT_DSA_PARAMETER_HEADER_V2
	{
		ULONG                   cbLength;
		ULONG                   dwMagic;
		ULONG                   cbKeyLength;
		HASHALGORITHM_ENUM      hashAlgorithm;
		DSAFIPSVERSION_ENUM     standardVersion;
		ULONG                   cbSeedLength;
		ULONG                   cbGroupSize;
		UCHAR                   Count[4];
	} BCRYPT_DSA_PARAMETER_HEADER_V2;

	// Operation types used in BCRYPT_MULTI_HASH_OPERATION structures
	typedef enum {
		BCRYPT_HASH_OPERATION_HASH_DATA = 1,
		BCRYPT_HASH_OPERATION_FINISH_HASH = 2,
	} BCRYPT_HASH_OPERATION_TYPE;

	typedef struct _BCRYPT_MULTI_HASH_OPERATION {
		ULONG                           iHash;          // index of hash object 
		BCRYPT_HASH_OPERATION_TYPE      hashOperation;  // operation to be performed
		_Field_size_(cbBuffer)  PUCHAR                          pbBuffer;       // data to be hashed, or result buffer
		ULONG                           cbBuffer;
	} BCRYPT_MULTI_HASH_OPERATION;

	// Enum to specify type of multi-operation is passed to BCryptProcesMultiOperations
	typedef enum {
		BCRYPT_OPERATION_TYPE_HASH = 1,     // structure type is BCRYPT_MULTI_HASH_OPERATION
	} BCRYPT_MULTI_OPERATION_TYPE;

	typedef struct _BCRYPT_MULTI_OBJECT_LENGTH_STRUCT
	{
		ULONG   cbPerObject;
		ULONG   cbPerElement;           // required size for N elements is (cbPerObject + N * cbPerElement)
	} BCRYPT_MULTI_OBJECT_LENGTH_STRUCT;

	//
	// Microsoft built-in providers.
	//

#define MS_PRIMITIVE_PROVIDER                   L"Microsoft Primitive Provider"
#define MS_PLATFORM_CRYPTO_PROVIDER             L"Microsoft Platform Crypto Provider"

	//
	// Common algorithm identifiers.
	//

#define BCRYPT_RSA_ALGORITHM                    L"RSA"
#define BCRYPT_RSA_SIGN_ALGORITHM               L"RSA_SIGN"
#define BCRYPT_DH_ALGORITHM                     L"DH"
#define BCRYPT_DSA_ALGORITHM                    L"DSA"
#define BCRYPT_RC2_ALGORITHM                    L"RC2"
#define BCRYPT_RC4_ALGORITHM                    L"RC4"
#define BCRYPT_AES_ALGORITHM                    L"AES"
#define BCRYPT_DES_ALGORITHM                    L"DES"
#define BCRYPT_DESX_ALGORITHM                   L"DESX"
#define BCRYPT_3DES_ALGORITHM                   L"3DES"
#define BCRYPT_3DES_112_ALGORITHM               L"3DES_112"
#define BCRYPT_MD2_ALGORITHM                    L"MD2"
#define BCRYPT_MD4_ALGORITHM                    L"MD4"
#define BCRYPT_MD5_ALGORITHM                    L"MD5"
#define BCRYPT_SHA1_ALGORITHM                   L"SHA1"
#define BCRYPT_SHA256_ALGORITHM                 L"SHA256"
#define BCRYPT_SHA384_ALGORITHM                 L"SHA384"
#define BCRYPT_SHA512_ALGORITHM                 L"SHA512"
#define BCRYPT_AES_GMAC_ALGORITHM               L"AES-GMAC"
#define BCRYPT_AES_CMAC_ALGORITHM               L"AES-CMAC"
#define BCRYPT_ECDSA_P256_ALGORITHM             L"ECDSA_P256"
#define BCRYPT_ECDSA_P384_ALGORITHM             L"ECDSA_P384"
#define BCRYPT_ECDSA_P521_ALGORITHM             L"ECDSA_P521"
#define BCRYPT_ECDH_P256_ALGORITHM              L"ECDH_P256"
#define BCRYPT_ECDH_P384_ALGORITHM              L"ECDH_P384"
#define BCRYPT_ECDH_P521_ALGORITHM              L"ECDH_P521"
#define BCRYPT_RNG_ALGORITHM                    L"RNG"
#define BCRYPT_RNG_FIPS186_DSA_ALGORITHM        L"FIPS186DSARNG"
#define BCRYPT_RNG_DUAL_EC_ALGORITHM            L"DUALECRNG"
#define BCRYPT_SP800108_CTR_HMAC_ALGORITHM      L"SP800_108_CTR_HMAC"
#define BCRYPT_SP80056A_CONCAT_ALGORITHM        L"SP800_56A_CONCAT"
#define BCRYPT_PBKDF2_ALGORITHM                 L"PBKDF2"
#define BCRYPT_CAPI_KDF_ALGORITHM               L"CAPI_KDF"

	//
	// Interfaces
	//

#define BCRYPT_CIPHER_INTERFACE                 0x00000001
#define BCRYPT_HASH_INTERFACE                   0x00000002
#define BCRYPT_ASYMMETRIC_ENCRYPTION_INTERFACE  0x00000003
#define BCRYPT_SECRET_AGREEMENT_INTERFACE       0x00000004
#define BCRYPT_SIGNATURE_INTERFACE              0x00000005
#define BCRYPT_RNG_INTERFACE                    0x00000006
#define BCRYPT_KEY_DERIVATION_INTERFACE         0x00000007


	//
	// Primitive algorithm provider functions.
	//

#define BCRYPT_ALG_HANDLE_HMAC_FLAG             0x00000008
#define BCRYPT_CAPI_AES_FLAG                    0x00000010
#define BCRYPT_HASH_REUSABLE_FLAG               0x00000020

#if (NTDDI_VERSION > NTDDI_WINBLUE || (NTDDI_VERSION == NTDDI_WINBLUE && defined(WINBLUE_KBSPRING14)))
#define BCRYPT_MULTI_FLAG                       0x00000040
#endif

	//
	// The BUFFERS_LOCKED flag used in BCryptEncrypt/BCryptDecrypt signals that
	// the pbInput and pbOutput buffers have been locked (see MmProbeAndLockPages)
	// and CNG may not lock the buffers again.
	// This flag applies only to kernel mode, it is ignored in user mode.
	//
#define BCRYPT_BUFFERS_LOCKED_FLAG      0x00000040

	//
	// The EXTENDED_KEYSIZE flag extends the supported set of key sizes.
	// 
	// The original design has a per-algorithm maximum key size, and 
	// BCryptGenerateSymmetricKey truncates any longer input to the maximum key size for that
	// algorithm. Some callers depend on this feature and pass in large buffers.
	// This makes it impossible to silently extend the supported key size without breaking
	// backward compatibility.
	// This flag indicates that the extended key size support is requested.
	// It has the following consequences:
	// - BCryptGetProperty will report a new maximum key size for BCRYPT_KEY_LENGTHS.
	// - BCryptGenerateSymmetricKey will support the longer key sizes.
	// - BCryptGenerateSymmetricKey will no longer truncate keys that are too long, but return an error instead.
	//
#if (NTDDI_VERSION >= NTDDI_WINBLUE)
#define BCRYPT_EXTENDED_KEYSIZE         0x00000080
#endif

	_Must_inspect_result_
		NTSTATUS
		WINAPI
		BCryptOpenAlgorithmProvider(
			_Out_       BCRYPT_ALG_HANDLE   *phAlgorithm,
			_In_        LPCWSTR pszAlgId,
			_In_opt_    LPCWSTR pszImplementation,
			_In_        ULONG   dwFlags);



	// AlgOperations flags for use with BCryptEnumAlgorithms()
#define BCRYPT_CIPHER_OPERATION                 0x00000001
#define BCRYPT_HASH_OPERATION                   0x00000002
#define BCRYPT_ASYMMETRIC_ENCRYPTION_OPERATION  0x00000004
#define BCRYPT_SECRET_AGREEMENT_OPERATION       0x00000008
#define BCRYPT_SIGNATURE_OPERATION              0x00000010
#define BCRYPT_RNG_OPERATION                    0x00000020
#define BCRYPT_KEY_DERIVATION_OPERATION         0x00000040

	// USE EXTREME CAUTION: editing comments that contain "certenrolls_*" tokens
	// could break building CertEnroll idl files:
	// certenrolls_begin -- BCRYPT_ALGORITHM_IDENTIFIER
	typedef struct _BCRYPT_ALGORITHM_IDENTIFIER
	{
		LPWSTR  pszName;
		ULONG   dwClass;
		ULONG   dwFlags;

	} BCRYPT_ALGORITHM_IDENTIFIER;
	// certenrolls_end

	_Must_inspect_result_
		NTSTATUS
		WINAPI
		BCryptEnumAlgorithms(
			_In_    ULONG   dwAlgOperations,
			_Out_   ULONG   *pAlgCount,
			_Out_   BCRYPT_ALGORITHM_IDENTIFIER **ppAlgList,
			_In_    ULONG   dwFlags);

	typedef struct _BCRYPT_PROVIDER_NAME
	{
		LPWSTR  pszProviderName;
	} BCRYPT_PROVIDER_NAME;

	_Must_inspect_result_
		NTSTATUS
		WINAPI
		BCryptEnumProviders(
			_In_    LPCWSTR pszAlgId,
			_Out_   ULONG   *pImplCount,
			_Out_   BCRYPT_PROVIDER_NAME    **ppImplList,
			_In_    ULONG   dwFlags);


	// Flags for use with BCryptGetProperty and BCryptSetProperty
#define BCRYPT_PUBLIC_KEY_FLAG                  0x00000001
#define BCRYPT_PRIVATE_KEY_FLAG                 0x00000002

	_Must_inspect_result_
		NTSTATUS
		WINAPI
		BCryptGetProperty(
			_In_                                        BCRYPT_HANDLE   hObject,
			_In_                                        LPCWSTR pszProperty,
			_Out_writes_bytes_to_opt_(cbOutput, *pcbResult) PUCHAR   pbOutput,
			_In_                                        ULONG   cbOutput,
			_Out_                                       ULONG   *pcbResult,
			_In_                                        ULONG   dwFlags);


	_Must_inspect_result_
		NTSTATUS
		WINAPI
		BCryptSetProperty(
			_Inout_                 BCRYPT_HANDLE   hObject,
			_In_                    LPCWSTR pszProperty,
			_In_reads_bytes_(cbInput)    PUCHAR   pbInput,
			_In_                    ULONG   cbInput,
			_In_                    ULONG   dwFlags);


	NTSTATUS
		WINAPI
		BCryptCloseAlgorithmProvider(
			_Inout_ BCRYPT_ALG_HANDLE   hAlgorithm,
			_In_    ULONG   dwFlags);


	VOID
		WINAPI
		BCryptFreeBuffer(
			_In_ PVOID   pvBuffer);


	//
	// Primitive encryption functions.
	//
	_Must_inspect_result_
		NTSTATUS
		WINAPI
		BCryptGenerateSymmetricKey(
			_Inout_                             BCRYPT_ALG_HANDLE   hAlgorithm,
			_Out_                               BCRYPT_KEY_HANDLE   *phKey,
			_Out_writes_bytes_all_opt_(cbKeyObject)  PUCHAR   pbKeyObject,
			_In_                                ULONG   cbKeyObject,
			_In_reads_bytes_(cbSecret)               PUCHAR   pbSecret,
			_In_                                ULONG   cbSecret,
			_In_                                ULONG   dwFlags);


	_Must_inspect_result_
		NTSTATUS
		WINAPI
		BCryptGenerateKeyPair(
			_Inout_ BCRYPT_ALG_HANDLE   hAlgorithm,
			_Out_   BCRYPT_KEY_HANDLE   *phKey,
			_In_    ULONG   dwLength,
			_In_    ULONG   dwFlags);


	_Must_inspect_result_
		NTSTATUS
		WINAPI
		BCryptEncrypt(
			_Inout_                                     BCRYPT_KEY_HANDLE hKey,
			_In_reads_bytes_opt_(cbInput)                    PUCHAR   pbInput,
			_In_                                        ULONG   cbInput,
			_In_opt_                                    VOID    *pPaddingInfo,
			_Inout_updates_bytes_opt_(cbIV)                    PUCHAR   pbIV,
			_In_                                        ULONG   cbIV,
			_Out_writes_bytes_to_opt_(cbOutput, *pcbResult) PUCHAR   pbOutput,
			_In_                                        ULONG   cbOutput,
			_Out_                                       ULONG   *pcbResult,
			_In_                                        ULONG   dwFlags);


	_Must_inspect_result_
		NTSTATUS
		WINAPI
		BCryptDecrypt(
			_Inout_                                     BCRYPT_KEY_HANDLE   hKey,
			_In_reads_bytes_opt_(cbInput)                    PUCHAR   pbInput,
			_In_                                        ULONG   cbInput,
			_In_opt_                                    VOID    *pPaddingInfo,
			_Inout_updates_bytes_opt_(cbIV)                    PUCHAR   pbIV,
			_In_                                        ULONG   cbIV,
			_Out_writes_bytes_to_opt_(cbOutput, *pcbResult) PUCHAR   pbOutput,
			_In_                                        ULONG   cbOutput,
			_Out_                                       ULONG   *pcbResult,
			_In_                                        ULONG   dwFlags);


	_Must_inspect_result_
		NTSTATUS
		WINAPI
		BCryptExportKey(
			_In_                                        BCRYPT_KEY_HANDLE   hKey,
			_In_opt_                                    BCRYPT_KEY_HANDLE   hExportKey,
			_In_                                        LPCWSTR pszBlobType,
			_Out_writes_bytes_to_opt_(cbOutput, *pcbResult) PUCHAR   pbOutput,
			_In_                                        ULONG   cbOutput,
			_Out_                                       ULONG   *pcbResult,
			_In_                                        ULONG   dwFlags);


	_Must_inspect_result_
		NTSTATUS
		WINAPI
		BCryptImportKey(
			_In_                                BCRYPT_ALG_HANDLE hAlgorithm,
			_In_opt_                            BCRYPT_KEY_HANDLE hImportKey,
			_In_                                LPCWSTR pszBlobType,
			_Out_                               BCRYPT_KEY_HANDLE *phKey,
			_Out_writes_bytes_all_opt_(cbKeyObject)  PUCHAR   pbKeyObject,
			_In_                                ULONG   cbKeyObject,
			_In_reads_bytes_(cbInput)                PUCHAR   pbInput,
			_In_                                ULONG   cbInput,
			_In_                                ULONG   dwFlags);


#define BCRYPT_NO_KEY_VALIDATION    0x00000008
	_Must_inspect_result_
		NTSTATUS
		WINAPI
		BCryptImportKeyPair(
			_In_                            BCRYPT_ALG_HANDLE hAlgorithm,
			_In_opt_                        BCRYPT_KEY_HANDLE hImportKey,
			_In_                            LPCWSTR pszBlobType,
			_Out_                           BCRYPT_KEY_HANDLE *phKey,
			_In_reads_bytes_(cbInput)            PUCHAR   pbInput,
			_In_                            ULONG   cbInput,
			_In_                            ULONG   dwFlags);


	_Must_inspect_result_
		NTSTATUS
		WINAPI
		BCryptDuplicateKey(
			_In_                                BCRYPT_KEY_HANDLE   hKey,
			_Out_                               BCRYPT_KEY_HANDLE   *phNewKey,
			_Out_writes_bytes_all_opt_(cbKeyObject)  PUCHAR   pbKeyObject,
			_In_                                ULONG   cbKeyObject,
			_In_                                ULONG   dwFlags);


	_Must_inspect_result_
		NTSTATUS
		WINAPI
		BCryptFinalizeKeyPair(
			_Inout_ BCRYPT_KEY_HANDLE   hKey,
			_In_    ULONG   dwFlags);


	NTSTATUS
		WINAPI
		BCryptDestroyKey(
			_Inout_ BCRYPT_KEY_HANDLE   hKey);


	NTSTATUS
		WINAPI
		BCryptDestroySecret(
			_Inout_ BCRYPT_SECRET_HANDLE   hSecret);


	_Must_inspect_result_
		NTSTATUS
		WINAPI
		BCryptSignHash(
			_In_                                        BCRYPT_KEY_HANDLE   hKey,
			_In_opt_                                    VOID    *pPaddingInfo,
			_In_reads_bytes_(cbInput)                        PUCHAR   pbInput,
			_In_                                        ULONG   cbInput,
			_Out_writes_bytes_to_opt_(cbOutput, *pcbResult) PUCHAR   pbOutput,
			_In_                                        ULONG   cbOutput,
			_Out_                                       ULONG   *pcbResult,
			_In_                                        ULONG   dwFlags);


	_Must_inspect_result_
		NTSTATUS
		WINAPI
		BCryptVerifySignature(
			_In_                        BCRYPT_KEY_HANDLE   hKey,
			_In_opt_                    VOID    *pPaddingInfo,
			_In_reads_bytes_(cbHash)         PUCHAR   pbHash,
			_In_                        ULONG   cbHash,
			_In_reads_bytes_(cbSignature)    PUCHAR   pbSignature,
			_In_                        ULONG   cbSignature,
			_In_                        ULONG   dwFlags);


	_Must_inspect_result_
		NTSTATUS
		WINAPI
		BCryptSecretAgreement(
			_In_    BCRYPT_KEY_HANDLE       hPrivKey,
			_In_    BCRYPT_KEY_HANDLE       hPubKey,
			_Out_   BCRYPT_SECRET_HANDLE    *phAgreedSecret,
			_In_    ULONG                   dwFlags);


	_Must_inspect_result_
		NTSTATUS
		WINAPI
		BCryptDeriveKey(
			_In_        BCRYPT_SECRET_HANDLE hSharedSecret,
			_In_        LPCWSTR              pwszKDF,
			_In_opt_    BCryptBufferDesc     *pParameterList,
			_Out_writes_bytes_to_opt_(cbDerivedKey, *pcbResult) PUCHAR pbDerivedKey,
			_In_        ULONG                cbDerivedKey,
			_Out_       ULONG                *pcbResult,
			_In_        ULONG                dwFlags);


	_Must_inspect_result_
		NTSTATUS
		WINAPI
		BCryptKeyDerivation(
			_In_        BCRYPT_KEY_HANDLE hKey,
			_In_opt_    BCryptBufferDesc     *pParameterList,
			_Out_writes_bytes_to_(cbDerivedKey, *pcbResult) PUCHAR pbDerivedKey,
			_In_        ULONG                cbDerivedKey,
			_Out_       ULONG                *pcbResult,
			_In_        ULONG                dwFlags);


	//
	// Primitive hashing functions.
	//
	_Must_inspect_result_
		NTSTATUS
		WINAPI
		BCryptCreateHash(
			_Inout_                             BCRYPT_ALG_HANDLE   hAlgorithm,
			_Out_                               BCRYPT_HASH_HANDLE  *phHash,
			_Out_writes_bytes_all_opt_(cbHashObject) PUCHAR   pbHashObject,
			_In_                                ULONG   cbHashObject,
			_In_reads_bytes_opt_(cbSecret)           PUCHAR   pbSecret,   // optional
			_In_                                ULONG   cbSecret,   // optional
			_In_                                ULONG   dwFlags);


	_Must_inspect_result_
		NTSTATUS
		WINAPI
		BCryptHashData(
			_Inout_                 BCRYPT_HASH_HANDLE  hHash,
			_In_reads_bytes_(cbInput)    PUCHAR   pbInput,
			_In_                    ULONG   cbInput,
			_In_                    ULONG   dwFlags);


	_Must_inspect_result_
		NTSTATUS
		WINAPI
		BCryptFinishHash(
			_Inout_                     BCRYPT_HASH_HANDLE hHash,
			_Out_writes_bytes_all_(cbOutput) PUCHAR   pbOutput,
			_In_                        ULONG   cbOutput,
			_In_                        ULONG   dwFlags);


#if (NTDDI_VERSION > NTDDI_WINBLUE || (NTDDI_VERSION == NTDDI_WINBLUE && defined(WINBLUE_KBSPRING14)))
	_Must_inspect_result_
		NTSTATUS
		WINAPI
		BCryptCreateMultiHash(
			_Inout_                                     BCRYPT_ALG_HANDLE   hAlgorithm,
			_Out_                                       BCRYPT_HASH_HANDLE *phHash,
			_In_                                        ULONG               nHashes,
			_Out_writes_bytes_all_opt_(cbHashObject)    PUCHAR              pbHashObject,
			_In_                                        ULONG               cbHashObject,
			_In_reads_bytes_opt_(cbSecret)              PUCHAR              pbSecret,   // optional
			_In_                                        ULONG               cbSecret,   // optional
			_In_                                        ULONG               dwFlags);

	_Must_inspect_result_
		NTSTATUS
		WINAPI
		BCryptProcessMultiOperations(
			_Inout_                         BCRYPT_HANDLE                   hObject,
			_In_                            BCRYPT_MULTI_OPERATION_TYPE     operationType,
			_In_reads_bytes_(cbOperations)  PVOID                           pOperations,
			_In_                            ULONG                           cbOperations,
			_In_                            ULONG                           dwFlags);
#endif


	_Must_inspect_result_
		NTSTATUS
		WINAPI
		BCryptDuplicateHash(
			_In_                                BCRYPT_HASH_HANDLE  hHash,
			_Out_                               BCRYPT_HASH_HANDLE  *phNewHash,
			_Out_writes_bytes_all_opt_(cbHashObject) PUCHAR   pbHashObject,
			_In_                                ULONG   cbHashObject,
			_In_                                ULONG   dwFlags);


	NTSTATUS
		WINAPI
		BCryptDestroyHash(
			_Inout_ BCRYPT_HASH_HANDLE  hHash);


	//
	// Primitive random number generation.
	//

	// Flags to BCryptGenRandom
#define BCRYPT_RNG_USE_ENTROPY_IN_BUFFER    0x00000001
#define BCRYPT_USE_SYSTEM_PREFERRED_RNG     0x00000002

	_Must_inspect_result_
		NTSTATUS
		WINAPI
		BCryptGenRandom(
			_In_opt_                        BCRYPT_ALG_HANDLE   hAlgorithm,
			_Inout_updates_bytes_all_(cbBuffer)   PUCHAR  pbBuffer,
			_In_                            ULONG   cbBuffer,
			_In_                            ULONG   dwFlags);


	//
	// Primitive key derivation functions.
	//
	_Must_inspect_result_
		NTSTATUS
		WINAPI
		BCryptDeriveKeyCapi(
			_In_                            BCRYPT_HASH_HANDLE  hHash,
			_In_opt_                        BCRYPT_ALG_HANDLE   hTargetAlg,
			_Out_writes_bytes_(cbDerivedKey)    PUCHAR              pbDerivedKey,
			_In_                            ULONG               cbDerivedKey,
			_In_                            ULONG               dwFlags);


	_Must_inspect_result_
		NTSTATUS
		WINAPI
		BCryptDeriveKeyPBKDF2(
			_In_                            BCRYPT_ALG_HANDLE   hPrf,
			_In_reads_bytes_opt_(cbPassword)   PUCHAR              pbPassword,
			_In_                            ULONG               cbPassword,
			_In_reads_bytes_opt_(cbSalt)       PUCHAR              pbSalt,
			_In_                            ULONG               cbSalt,
			_In_                            ULONGLONG           cIterations,
			_Out_writes_bytes_(cbDerivedKey)    PUCHAR              pbDerivedKey,
			_In_                            ULONG               cbDerivedKey,
			_In_                            ULONG               dwFlags);


	//
	// Interface version control...
	//
	typedef struct _BCRYPT_INTERFACE_VERSION
	{
		USHORT MajorVersion;
		USHORT MinorVersion;

	} BCRYPT_INTERFACE_VERSION, *PBCRYPT_INTERFACE_VERSION;

#define BCRYPT_MAKE_INTERFACE_VERSION(major,minor) {(USHORT)major, (USHORT)minor}

#define BCRYPT_IS_INTERFACE_VERSION_COMPATIBLE(loader, provider)    \
    ((loader).MajorVersion <= (provider).MajorVersion)

	//
	// Primitive provider interfaces.
	//

#define BCRYPT_CIPHER_INTERFACE_VERSION_1    BCRYPT_MAKE_INTERFACE_VERSION(1,0)


#define BCRYPT_HASH_INTERFACE_VERSION_1         BCRYPT_MAKE_INTERFACE_VERSION(1,0)

#if (NTDDI_VERSION > NTDDI_WINBLUE || (NTDDI_VERSION == NTDDI_WINBLUE && defined(WINBLUE_KBSPRING14)))
#define BCRYPT_HASH_INTERFACE_MAJORVERSION_2    2
#define BCRYPT_HASH_INTERFACE_VERSION_2         BCRYPT_MAKE_INTERFACE_VERSION(BCRYPT_HASH_INTERFACE_MAJORVERSION_2,0)
#endif


#define BCRYPT_ASYMMETRIC_ENCRYPTION_INTERFACE_VERSION_1    BCRYPT_MAKE_INTERFACE_VERSION(1,0)


#define BCRYPT_SECRET_AGREEMENT_INTERFACE_VERSION_1    BCRYPT_MAKE_INTERFACE_VERSION(1,0)


#define BCRYPT_SIGNATURE_INTERFACE_VERSION_1    BCRYPT_MAKE_INTERFACE_VERSION(1,0)


#define BCRYPT_RNG_INTERFACE_VERSION_1    BCRYPT_MAKE_INTERFACE_VERSION(1,0)


	//////////////////////////////////////////////////////////////////////////////
	// CryptoConfig Definitions //////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////

	// Interface registration flags
#define CRYPT_MIN_DEPENDENCIES      (0x00000001)
#define CRYPT_PROCESS_ISOLATE       (0x00010000) // User-mode only

	// Processor modes supported by a provider
	//
	// (Valid for BCryptQueryProviderRegistration and BCryptResolveProviders):
	//
#define CRYPT_UM                    (0x00000001)    // User mode only
#define CRYPT_KM                    (0x00000002)    // Kernel mode only
#define CRYPT_MM                    (0x00000003)    // Multi-mode: Must support BOTH UM and KM
	//
	// (Valid only for BCryptQueryProviderRegistration):
	//
#define CRYPT_ANY                   (0x00000004)    // Wildcard: Either UM, or KM, or both


	// Write behavior flags
#define CRYPT_OVERWRITE             (0x00000001)

	// Configuration tables
#define CRYPT_LOCAL                 (0x00000001)
#define CRYPT_DOMAIN                (0x00000002)

	// Context configuration flags
#define CRYPT_EXCLUSIVE             (0x00000001)
#define CRYPT_OVERRIDE              (0x00010000) // Enterprise table only

	// Resolution and enumeration flags
#define CRYPT_ALL_FUNCTIONS         (0x00000001)
#define CRYPT_ALL_PROVIDERS         (0x00000002)

	// Priority list positions
#define CRYPT_PRIORITY_TOP          (0x00000000)
#define CRYPT_PRIORITY_BOTTOM       (0xFFFFFFFF)

	// Default system-wide context
#define CRYPT_DEFAULT_CONTEXT       L"Default"

	//////////////////////////////////////////////////////////////////////////////
	// CryptoConfig Structures ///////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////

	//
	// Provider Registration Structures
	//

	typedef struct _CRYPT_INTERFACE_REG
	{
		ULONG dwInterface;
		ULONG dwFlags;

		ULONG cFunctions;
		PWSTR *rgpszFunctions;
	}
	CRYPT_INTERFACE_REG, *PCRYPT_INTERFACE_REG;

	typedef struct _CRYPT_IMAGE_REG
	{
		PWSTR pszImage;

		ULONG cInterfaces;
		PCRYPT_INTERFACE_REG *rgpInterfaces;
	}
	CRYPT_IMAGE_REG, *PCRYPT_IMAGE_REG;

	typedef struct _CRYPT_PROVIDER_REG
	{
		ULONG cAliases;
		PWSTR *rgpszAliases;

		PCRYPT_IMAGE_REG pUM;
		PCRYPT_IMAGE_REG pKM;
	}
	CRYPT_PROVIDER_REG, *PCRYPT_PROVIDER_REG;

	typedef struct _CRYPT_PROVIDERS
	{
		ULONG cProviders;
		PWSTR *rgpszProviders;
	}
	CRYPT_PROVIDERS, *PCRYPT_PROVIDERS;

	//
	// Context Configuration Structures
	//

	typedef struct _CRYPT_CONTEXT_CONFIG
	{
		ULONG dwFlags;
		ULONG dwReserved;
	}
	CRYPT_CONTEXT_CONFIG, *PCRYPT_CONTEXT_CONFIG;

	typedef struct _CRYPT_CONTEXT_FUNCTION_CONFIG
	{
		ULONG dwFlags;
		ULONG dwReserved;
	}
	CRYPT_CONTEXT_FUNCTION_CONFIG, *PCRYPT_CONTEXT_FUNCTION_CONFIG;

	typedef struct _CRYPT_CONTEXTS
	{
		ULONG cContexts;
		PWSTR *rgpszContexts;
	}
	CRYPT_CONTEXTS, *PCRYPT_CONTEXTS;

	typedef struct _CRYPT_CONTEXT_FUNCTIONS
	{
		ULONG cFunctions;
		PWSTR *rgpszFunctions;
	}
	CRYPT_CONTEXT_FUNCTIONS, *PCRYPT_CONTEXT_FUNCTIONS;

	typedef struct _CRYPT_CONTEXT_FUNCTION_PROVIDERS
	{
		ULONG cProviders;
		PWSTR *rgpszProviders;
	}
	CRYPT_CONTEXT_FUNCTION_PROVIDERS, *PCRYPT_CONTEXT_FUNCTION_PROVIDERS;

	//
	// Provider Resolution Structures
	//

	typedef struct _CRYPT_PROPERTY_REF
	{
		PWSTR pszProperty;

		ULONG cbValue;
		PUCHAR pbValue;
	}
	CRYPT_PROPERTY_REF, *PCRYPT_PROPERTY_REF;

	typedef struct _CRYPT_IMAGE_REF
	{
		PWSTR pszImage;
		ULONG dwFlags;
	}
	CRYPT_IMAGE_REF, *PCRYPT_IMAGE_REF;

	typedef struct _CRYPT_PROVIDER_REF
	{
		ULONG dwInterface;
		PWSTR pszFunction;
		PWSTR pszProvider;

		ULONG cProperties;
		PCRYPT_PROPERTY_REF *rgpProperties;

		PCRYPT_IMAGE_REF pUM;
		PCRYPT_IMAGE_REF pKM;
	}
	CRYPT_PROVIDER_REF, *PCRYPT_PROVIDER_REF;

	typedef struct _CRYPT_PROVIDER_REFS
	{
		ULONG cProviders;
		PCRYPT_PROVIDER_REF *rgpProviders;
	}
	CRYPT_PROVIDER_REFS, *PCRYPT_PROVIDER_REFS;

	//////////////////////////////////////////////////////////////////////////////
	// CryptoConfig Functions ////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////

#ifndef KERNEL_MODE_CNG


	_Must_inspect_result_
		NTSTATUS
		WINAPI
		BCryptQueryProviderRegistration(
			_In_ LPCWSTR pszProvider,
			_In_ ULONG dwMode,
			_In_ ULONG dwInterface,
			_Inout_ ULONG* pcbBuffer,
			_Inout_
			_When_(_Old_(*ppBuffer) != NULL, _At_(*ppBuffer, _Out_writes_bytes_to_(*pcbBuffer, *pcbBuffer)))
			_When_(_Old_(*ppBuffer) == NULL, _Outptr_result_bytebuffer_all_(*pcbBuffer))
			PCRYPT_PROVIDER_REG *ppBuffer);

	_Must_inspect_result_
		NTSTATUS
		WINAPI
		BCryptEnumRegisteredProviders(
			_Inout_ ULONG* pcbBuffer,
			_Inout_
			_When_(_Old_(*ppBuffer) != NULL, _At_(*ppBuffer, _Out_writes_bytes_to_(*pcbBuffer, *pcbBuffer)))
			_When_(_Old_(*ppBuffer) == NULL, _Outptr_result_bytebuffer_all_(*pcbBuffer))
			PCRYPT_PROVIDERS *ppBuffer);

	//
	// Context Configuration Functions
	//
	_Must_inspect_result_
		NTSTATUS
		WINAPI
		BCryptCreateContext(
			_In_ ULONG dwTable,
			_In_ LPCWSTR pszContext,
			_In_opt_ PCRYPT_CONTEXT_CONFIG pConfig); // Optional

	_Must_inspect_result_
		NTSTATUS
		WINAPI
		BCryptDeleteContext(
			_In_ ULONG dwTable,
			_In_ LPCWSTR pszContext);

	_Must_inspect_result_
		NTSTATUS
		WINAPI
		BCryptEnumContexts(
			_In_ ULONG dwTable,
			_Inout_ ULONG* pcbBuffer,
			_Inout_
			_When_(_Old_(*ppBuffer) != NULL, _At_(*ppBuffer, _Out_writes_bytes_to_(*pcbBuffer, *pcbBuffer)))
			_When_(_Old_(*ppBuffer) == NULL, _Outptr_result_bytebuffer_all_(*pcbBuffer))
			PCRYPT_CONTEXTS *ppBuffer);

	_Must_inspect_result_
		NTSTATUS
		WINAPI
		BCryptConfigureContext(
			_In_ ULONG dwTable,
			_In_ LPCWSTR pszContext,
			_In_ PCRYPT_CONTEXT_CONFIG pConfig);

	_Must_inspect_result_
		NTSTATUS
		WINAPI
		BCryptQueryContextConfiguration(
			_In_ ULONG dwTable,
			_In_ LPCWSTR pszContext,
			_Inout_ ULONG* pcbBuffer,
			_Inout_
			_When_(_Old_(*ppBuffer) != NULL, _At_(*ppBuffer, _Out_writes_bytes_to_(*pcbBuffer, *pcbBuffer)))
			_When_(_Old_(*ppBuffer) == NULL, _Outptr_result_bytebuffer_all_(*pcbBuffer))
			PCRYPT_CONTEXT_CONFIG *ppBuffer);

	_Must_inspect_result_
		NTSTATUS
		WINAPI
		BCryptAddContextFunction(
			_In_ ULONG dwTable,
			_In_ LPCWSTR pszContext,
			_In_ ULONG dwInterface,
			_In_ LPCWSTR pszFunction,
			_In_ ULONG dwPosition);

	_Must_inspect_result_
		NTSTATUS
		WINAPI
		BCryptRemoveContextFunction(
			_In_ ULONG dwTable,
			_In_ LPCWSTR pszContext,
			_In_ ULONG dwInterface,
			_In_ LPCWSTR pszFunction);

	_Must_inspect_result_
		NTSTATUS
		WINAPI
		BCryptEnumContextFunctions(
			_In_ ULONG dwTable,
			_In_ LPCWSTR pszContext,
			_In_ ULONG dwInterface,
			_Inout_ ULONG* pcbBuffer,
			_Inout_
			_When_(_Old_(*ppBuffer) != NULL, _At_(*ppBuffer, _Out_writes_bytes_to_(*pcbBuffer, *pcbBuffer)))
			_When_(_Old_(*ppBuffer) == NULL, _Outptr_result_bytebuffer_all_(*pcbBuffer))
			PCRYPT_CONTEXT_FUNCTIONS *ppBuffer);

	_Must_inspect_result_
		NTSTATUS
		WINAPI
		BCryptConfigureContextFunction(
			_In_ ULONG dwTable,
			_In_ LPCWSTR pszContext,
			_In_ ULONG dwInterface,
			_In_ LPCWSTR pszFunction,
			_In_ PCRYPT_CONTEXT_FUNCTION_CONFIG pConfig);

	_Must_inspect_result_
		NTSTATUS
		WINAPI
		BCryptQueryContextFunctionConfiguration(
			_In_ ULONG dwTable,
			_In_ LPCWSTR pszContext,
			_In_ ULONG dwInterface,
			_In_ LPCWSTR pszFunction,
			_Inout_ ULONG* pcbBuffer,
			_Inout_
			_When_(_Old_(*ppBuffer) != NULL, _At_(*ppBuffer, _Out_writes_bytes_to_(*pcbBuffer, *pcbBuffer)))
			_When_(_Old_(*ppBuffer) == NULL, _Outptr_result_bytebuffer_all_(*pcbBuffer))
			PCRYPT_CONTEXT_FUNCTION_CONFIG *ppBuffer);


	_Must_inspect_result_
		NTSTATUS
		WINAPI
		BCryptEnumContextFunctionProviders(
			_In_ ULONG dwTable,
			_In_ LPCWSTR pszContext,
			_In_ ULONG dwInterface,
			_In_ LPCWSTR pszFunction,
			_Inout_ ULONG* pcbBuffer,
			_Inout_
			_When_(_Old_(*ppBuffer) != NULL, _At_(*ppBuffer, _Out_writes_bytes_to_(*pcbBuffer, *pcbBuffer)))
			_When_(_Old_(*ppBuffer) == NULL, _Outptr_result_bytebuffer_all_(*pcbBuffer))
			PCRYPT_CONTEXT_FUNCTION_PROVIDERS *ppBuffer);

	_Must_inspect_result_
		NTSTATUS
		WINAPI
		BCryptSetContextFunctionProperty(
			_In_ ULONG dwTable,
			_In_ LPCWSTR pszContext,
			_In_ ULONG dwInterface,
			_In_ LPCWSTR pszFunction,
			_In_ LPCWSTR pszProperty,
			_In_ ULONG cbValue,
			_In_reads_bytes_opt_(cbValue) PUCHAR pbValue);

	_Must_inspect_result_
		NTSTATUS
		WINAPI
		BCryptQueryContextFunctionProperty(
			_In_ ULONG dwTable,
			_In_ LPCWSTR pszContext,
			_In_ ULONG dwInterface,
			_In_ LPCWSTR pszFunction,
			_In_ LPCWSTR pszProperty,
			_Inout_ ULONG* pcbValue,
			_Inout_
			_When_(_Old_(*ppbValue) != NULL, _At_(*ppbValue, _Out_writes_bytes_to_(*pcbValue, *pcbValue)))
			_When_(_Old_(*ppbValue) == NULL, _Outptr_result_bytebuffer_all_(*pcbValue))
			PUCHAR *ppbValue);

#endif //#ifndef KERNEL_MODE_CNG

	//
	// Configuration Change Notification Functions
	//

#ifdef KERNEL_MODE_CNG
	_Must_inspect_result_
		NTSTATUS
		WINAPI
		BCryptRegisterConfigChangeNotify(
			_In_ PRKEVENT pEvent);

#else
	_Must_inspect_result_
		NTSTATUS
		WINAPI
		BCryptRegisterConfigChangeNotify(
			_Out_ HANDLE *phEvent);
#endif

#ifdef KERNEL_MODE_CNG
	NTSTATUS
		WINAPI
		BCryptUnregisterConfigChangeNotify(
			_In_ PRKEVENT pEvent);

#else
	NTSTATUS
		WINAPI
		BCryptUnregisterConfigChangeNotify(
			_In_ HANDLE hEvent);
#endif

	//
	// Provider Resolution Functions
	//
	_Must_inspect_result_
		NTSTATUS WINAPI
		BCryptResolveProviders(
			_In_opt_ LPCWSTR pszContext,
			_In_opt_ ULONG dwInterface,
			_In_opt_ LPCWSTR pszFunction,
			_In_opt_ LPCWSTR pszProvider,
			_In_ ULONG dwMode,
			_In_ ULONG dwFlags,
			_Inout_ ULONG* pcbBuffer,
			_Inout_
			_When_(_Old_(*ppBuffer) != NULL, _At_(*ppBuffer, _Out_writes_bytes_to_(*pcbBuffer, *pcbBuffer)))
			_When_(_Old_(*ppBuffer) == NULL, _Outptr_result_bytebuffer_all_(*pcbBuffer))
			PCRYPT_PROVIDER_REFS *ppBuffer);

	//
	// Miscellaneous queries about the crypto environment
	//
	NTSTATUS
		WINAPI
		BCryptGetFipsAlgorithmMode(
			_Out_ BOOLEAN *pfEnabled
		);


#endif /* WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) */
#pragma endregion

#ifdef __cplusplus
}
#endif

#endif // __BCRYPT_H__

