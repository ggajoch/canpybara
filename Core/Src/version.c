#include "can.h"

void canpybara_version_report(void)
{
	static CanTxMsgTypeDef can_tx;
	can_tx.StdId = CANPYBARA_REPORT_VERSIONINFO;
	can_tx.ExtId = 0;
	can_tx.IDE = CAN_ID_STD;
	can_tx.RTR = CAN_RTR_DATA;

	can_tx.DLC = 2;
	can_tx.Data[0] = 0;
	can_tx.Data[1] = 1;

	canpybara_can_tx(&can_tx);
}
