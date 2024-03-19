#include "../services.h"
#include "../../UDS/UDS.h"

/**
 * @brief Wrapper function for the SID 0x27, which is the security access service.
 * @param rx The UDS packet to process.
 * @param resp_data The returning response data.
 * @param idx The length of the response data.
 * @return True if the operation was successful.
 */
bool handle_security_access(UDS_Packet *rx, uInt8 *resp_data, uInt16 *idx);

/**
 * @brief Security check for the security access function.
 * @param rx The UDS packet to process.
 * @param resp_data The returning response data.
 * @param idx The length of the response data.
 * @param dgSess The required diagnostic session to be in.
 * @param secAccess The required security level to be in before executing the function.
 * @return True if the security check was passed.
 */
bool security_check_security_access(UDS_Packet *rx, uInt8 *resp_data, uInt16 *idx, uInt8 dgSess, uInt8 secAccess);
