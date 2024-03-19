#include "../../common.h"
#include "../../UDS/UDS.h"
#include "../services.h"

/**
 * @brief Wrapper function for the SID 0x10, which is the diagnostic session control.
 * @param rx The UDS packet to process.
 * @param resp_data The returning response data to be edited.
 * @param idx The length of the the response data.
 * @return True if the operation was successful.
 */
bool handle_diag_sess_cntl(UDS_Packet *rx, uInt8 *resp_data, uInt16 *idx);
