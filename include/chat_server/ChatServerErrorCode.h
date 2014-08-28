#ifndef CHAT_SERVER_CHATSERVERERRORCODE_H
#define CHAT_SERVER_CHATSERVERERRORCODE_H

#define ESUCCEED             0  /** 操作成功 */
#define EFAILED             -1  /** 操作失败 */
#define EINTNLSRVFAULT      -2  /** 内部服务器间调用错误 */

#define EROOMNAMEEXIST      -3  /** 房间名已存在 */
#define EROOMPASSWDERR      -4  /** 房间密码错误，无法加入 */
#define EROOMNAMENOTEXIST   -5  /** 房间不存在，无法加入 */
#define EALREADYINTHISROOM  -6  /** 已在该房间 */
#define ELOGINOTHERDEVICE   -7  /** 账号在其它设备登录，被踢下线 */
#define EHAVENOTLOGIN       -8  /** 尚未登录，不能执行其它操作 */
#define EALREADYLOGIN       -9  /** 已登录，不能重复登录 */
#define EROOMFULL           -10 /** 该房间已满，无法加入 */
#define ENOROOMRESOURCE     -11 /** 服务器上房间已满，不能再创建房间 */

#endif /* CHAT_SERVER_CHATSERVERERRORCODE_H */
