/**
	LWIP配置文件,通过此文件可以选择LWIP的功能,
	此文件基于opt.h文件,opt.h是LWIP"出厂"时原装的文件,由Adam等人编写
	而lwipopt.h的作者是ST的,它集合了lwip中常常需要改动的地方
	和针对ST所特有的配置选项添加进来了,这两个文件里边都配置了的
	选项以后者为准,不是共有的选项以他们各自的配置为准
*/

#ifndef __LWIPOPTS_H__
#define __LWIPOPTS_H__

#ifndef TCPIP_THREAD_PRIO
#define TCPIP_THREAD_PRIO        (configMAX_PRIORITIES - 1)                //内核任务优先级
#endif
//*****************************************************************************
//
// ---------- SNMP options ----------
//
////*****************************************************************************
//#define LWIP_SNMP                       0      　　//SNMP协议启用
//#if LWIP_SNMP
//#define SNMP_CONCURRENT_REQUESTS     1            //SNMP当前允许请求数量
//#define SNMP_TRAP_DESTINATIONS       1            //SNMP trap目的地址数目
//#define SNMP_PRIVATE_MIB             1      　　　 //SNMP 私有节点设置允许
//#define SNMP_SAFE_REQUESTS           1            //仅回复SNMP安全请求
//#endif


#define SYS_LIGHTWEIGHT_PROT    1
//NO_SYS==0:使用操作系统
#define NO_SYS                  0  //不使用UCOS操作系统



//#define MEM_LIBC_MALLOC                 0                //采用LWIP自带函数分配动态内存

//使用4字节对齐模式
#define MEM_ALIGNMENT           4  

//MEM_SIZE:heap内存的大小,如果在应用中有大量数据发送的话这个值最好设置大一点 
#define MEM_SIZE                8*1024 //内存堆大小

//#define MEMP_SANITY_CHECK               0                //mem_free调用后检查链表是否有循环 by zc
//#define MEMP_OVERFLOW_CHECK             0                //lwip堆内存溢出检查 
//#define MEM_USE_POOLS                   0                //内存堆不通过内存池分配
//#define MEM_USE_POOLS_TRY_BIGGER_POOL   0             　 //申请内存失败不选择大内存池
//#define MEMP_USE_CUSTOM_POOLS           0                //同上

//MEMP_NUM_PBUF:memp结构的pbuf数量,如果应用从ROM或者静态存储区发送大量数据时,这个值应该设置大一点
#define MEMP_NUM_PBUF           60
//#define MEMP_NUM_RAW_PCB        4    　　 //RAW连接的PCB数目(需要LWIP RAW有效)

//MEMP_NUM_UDP_PCB:UDP协议控制块(PCB)数量.每个活动的UDP"连接"需要一个PCB.
#define MEMP_NUM_UDP_PCB        6       //能够同时连接的UDP的PCB数目

//MEMP_NUM_TCP_PCB:同时建立激活的TCP数量
#define MEMP_NUM_TCP_PCB        10
//#define MEMP_NUM_TCP_PCB                    (TCP_WND + TCP_SND_BUF)/TCP_MSS  //能够同时连接的TCP的PCB数目 12

//MEMP_NUM_TCP_PCB_LISTEN:能够监听的TCP连接数量
//#define MEMP_NUM_TCP_PCB_LISTEN 			 6
#define MEMP_NUM_TCP_PCB_LISTEN              6       //(TCP_WND + TCP_SND_BUF)/TCP_MSS  //能够同时监听的TCP的PCB数目

//MEMP_NUM_TCP_SEG:最多同时在队列中的TCP段数量
#define MEMP_NUM_TCP_SEG        15
//#define MEMP_NUM_TCP_SEG                    40  　　　//80 能够同时在队列里的TCP的PCB数目

//#define MEMP_NUM_REASSDATA                   8       //最大同时等待重装的IP包数目，是整个IP包，不是IP分片
//#define MEMP_NUM_ARP_QUEUE                  30       //最大等待回复ARP请求的数目(ARP_QUEUEING有效)
//#define MEMP_NUM_IGMP_GROUP                  8       //多播组同时接收数据的最大成员数目(LWIP_IGMP有效)

//MEMP_NUM_SYS_TIMEOUT:能够同时激活的timeout个数
//#define MEMP_NUM_SYS_TIMEOUT    8
#define MEMP_NUM_SYS_TIMEOUT                20       //能够同时激活的超时连接数目(NO_SYS==0有戏)

#define MEMP_NUM_NETBUF                     10       //netbufs结构的数目
#define MEMP_NUM_NETCONN                    16       //netconns结构的数目
#define MEMP_NUM_TCPIP_MSG_API              40       //tcpip_msg结构的最大数目，用于callback和API的通讯 by zc
#define MEMP_NUM_TCPIP_MSG_INPKT            40       //tcpip_msg接受数据包结构的最大数目 by zc

/* ---------- Pbuf选项---------- */
//PBUF_POOL_SIZE:pbuf内存池个数. 
//#define PBUF_POOL_SIZE          20
#define PBUF_POOL_SIZE                      20       //内存池数量(小内存减小该选项可大大减小内存占用)

//PBUF_POOL_BUFSIZE:每个pbuf内存池大小. 
#define PBUF_POOL_BUFSIZE       512



//*****************************************************************************
//
// ---------- ARP options ----------
//
//*****************************************************************************
#define LWIP_ARP                            1        //ARP协议允许 
//#define ARP_TABLE_SIZE                      10  　　　//ARP维护的表项大小
//#define ARP_QUEUEING                        1   　　　//硬件地址解析时，将发送数据包计入队列
//#define ETHARP_TRUST_IP_MAC                 1   　　　//所有IP数据包都会直接引起ARP table的更新，
//                                                     //为0则非表项内IP-MAC关系会引起ARP请求,可以避免IP欺骗，不过会造成延时
//#define ETHARP_SUPPORT_VLAN                 0   　　　//非虚拟局域网，为1则仅虚拟局域网通讯有效
// 
 

//*****************************************************************************
//
// ---------- IP options ----------
//
//*****************************************************************************
#define IP_FORWARD                          0    //不允许不匹配数据包转发,多接口时为1
#define IP_OPTIONS_ALLOWED                  1    //带IP选项数据包允许 为0则丢弃所有IP数据包
#define IP_REASSEMBLY                       1    //允许接收IP包分片包(为0不允许，不能够接收大于MTU的包)
#define IP_FRAG                             1    //允许发送IP包分片包
#define IP_REASS_MAXAGE                     3    //允许接收的最大分段数
#define IP_REASS_MAX_PBUFS                  10   //最大允许存在的IP分片包占用的内存池个数
#define IP_FRAG_USES_STATIC_BUF             1    //IP分片使用静态缓冲区
#define IP_FRAG_MAX_MTU                     1500 //IP分片最大缓冲数量
#define IP_DEFAULT_TTL                      255  //IP数据包最大经历设备数目
#define IP_SOF_BROADCAST                    0    //IP发送广播包过滤
#define IP_SOF_BROADCAST_RECV               0    //IP接收广播包过滤

/* ---------- TCP选项---------- */
#define LWIP_TCP                1  //为1是使用TCP
#define TCP_TTL                  (IP_DEFAULT_TTL)                   //TCP数据包传输次数255//生存时间

/*当TCP的数据段超出队列时的控制位,当设备的内存过小的时候此项应为0*/
#undef TCP_QUEUE_OOSEQ
#define TCP_QUEUE_OOSEQ         0				//这里可能需要配置为1
//最大TCP分段
#define TCP_MSS                 (1500 - 40)	  //TCP_MSS = (MTU - IP报头大小 - TCP报头大小
#define TCP_CALCULATE_EFF_SEND_MSS          1                                  //tcp预计发送的分段长度，为1则根据窗口大小分配

//TCP发送缓冲区大小(bytes).
#define TCP_SND_BUF             (4*TCP_MSS)		//(8*TCP_MSS)     //TCP发送缓冲区  zc 7.1

//TCP_SND_QUEUELEN: TCP发送缓冲区大小(pbuf).这个值最小为(2 * TCP_SND_BUF/TCP_MSS) */
#define TCP_SND_QUEUELEN        (2* TCP_SND_BUF/TCP_MSS)	//(4*(TCP_SND_BUF/TCP_MSS))          //TCP发送队列长度
#define TCP_SNDLOWAT            (TCP_SND_BUF/4)                    //TCP可发送数据长度
#define TCP_LISTEN_BACKLOG       1                                  //TCP多连接允许
#define TCP_DEFAULT_LISTEN_BACKLOG          0xff                               //TCP连接和半连接的总数
#define LWIP_TCP_TIMESTAMPS                 0                                  //TCP时间戳选项
#define TCP_WND_UPDATE_THRESHOLD            (TCP_WND / 4)                      //TCP窗口更新阈值

//TCP发送窗口
#define TCP_WND                  4*TCP_MSS                          //tcp窗口大小
#define TCP_MAXRTX               12                                 //最大允许重传TCP数据包数目
#define TCP_SYNMAXRTX            6                                  //最大允许重传SYN连接包数目

//TCP定时器设置选项
//#define  TCP_MSL                             10000UL                            //TCP连接存在时间 单位ms
//#define  TCP_FIN_WAIT_TIMEOUT                20000UL                            //FIN等到ACK应答时间 单位ms
//#define  TCP_TMR_INTERVAL                    20                                 //TCP定时器计数间隔 20ms

/* ---------- ICMP选项---------- */

#define LWIP_ICMP                           1    //开启ping包接收/发送
#define ICMP_TTL                            (IP_DEFAULT_TTL) //ping包最大经历设备数目
#define LWIP_BROADCAST_PING                 0    //不回复广播ping包
#define LWIP_MULTICAST_PING                 0    //不回复多播ping包

//*****************************************************************************
//
// ---------- RAW选项 ----------
//
//*****************************************************************************
//#define LWIP_RAW                            0              //无操作系统基于回调函数驱动
//#define RAW_TTL                           (IP_DEFAULT_TTL) //应用层数据传输次数(基于IP层的TTL)

/* ---------- DHCP选项---------- */
//当使用DHCP时此位应该为1,LwIP 0.5.1版本中没有DHCP服务.
#define LWIP_DHCP               			1	 //动态主机协议配置(为1时)
//#define LWIP_AUTOIP                         0    //动态主机IP地址配置(为1时)
//#define LWIP_DHCP_AUTOIP_COOP               0    //允许上述两种配置同时存在于1个接口(为1时)
//#define LWIP_DHCP_AUTOIP_COOP_TRIES         9    //DHCP分配允许失败次数，失败则使用AUTOUP

//*****************************************************************************
//
// ---------- IGMP选项 ----------
//
//*****************************************************************************
#define LWIP_IGMP                           0            //LWIP组管理协议

//*****************************************************************************
//
// ---------- DNS选项 -----------
//
//*****************************************************************************
#define LWIP_DNS                            0     //域名服务器模块(依托UDP协议)
#define DNS_TABLE_SIZE                      4     //域名服务器维护的最大入口数目
#define DNS_MAX_NAME_LENGTH                 256   //域名服务器主机地址最大长度
#define DNS_MAX_SERVERS                     2     //域名服务器最大服务数目
#define DNS_DOES_NAME_CHECK                 1     //查询域名服务器时检测地址名
#define DNS_USES_STATIC_BUF                 1     //域名服务器使用静态地址
#define DNS_MSG_SIZE                        512   //域名服务器接收最大通讯数据长度
#define DNS_LOCAL_HOSTLIST                  0     //在本地维护域名服务器主机-地址表(为1时)
#define DNS_LOCAL_HOSTLIST_IS_DYNAMIC       0     //主机-地址表实时更新(为1时)


/* ---------- UDP选项 ---------- */ 
#define LWIP_UDP                               1            //启用UDP协议(snmp协议基于此)
#define LWIP_UDPLITE                           1            //UDP协议启用精简版
#define UDP_TTL                                (IP_DEFAULT_TTL) //UDP数据包传输次数
#define LWIP_NETBUF_RECVINFO                   0            //接收到的数据包除首个外其它不附加目的地址和端口


/* ---------- Statistics options ---------- */
#define LWIP_STATS 0				//LWIP统计选项
#define LWIP_PROVIDE_ERRNO 1

//*****************************************************************************
//         
// ---------- Internal Memory Pool Sizes ----------
//
//*****************************************************************************
 #define  PBUF_LINK_HLEN                 14           //为处理以太网头申请的包长度(本地MAC地址+远端MAC地址+协议类型) 6+6+2                          #define  PBUF_POOL_BUFSIZE             256           //单个内存池长度，要考虑到4字节对齐和最佳大小
 #define  ETH_PAD_SIZE                  0             //以太网填充长度，stm32设置为0 根据发送包判断
 
//关系不大.. 默认即可

/*
   --------------------------------------
   ---------- 帧校验和选项----------
   --------------------------------------
*/

//STM32F4x7允许通过硬件识别和计算IP,UDP和ICMP的帧校验和

#define CHECKSUM_BY_HARDWARE //定义CHECKSUM_BY_HARDWARE,使用硬件帧校验


#ifdef CHECKSUM_BY_HARDWARE
  //CHECKSUM_GEN_IP==0: 硬件生成IP数据包的帧校验和
  #define CHECKSUM_GEN_IP                 0
  //CHECKSUM_GEN_UDP==0: 硬件生成UDP数据包的帧校验和
  #define CHECKSUM_GEN_UDP                0
  //CHECKSUM_GEN_TCP==0: 硬件生成TCP数据包的帧校验和
  #define CHECKSUM_GEN_TCP                0 
  //CHECKSUM_CHECK_IP==0: 硬件检查输入的IP数据包帧校验和
  #define CHECKSUM_CHECK_IP               0
  //CHECKSUM_CHECK_UDP==0: 硬件检查输入的UDP数据包帧校验和
  #define CHECKSUM_CHECK_UDP              0
  //CHECKSUM_CHECK_TCP==0: 硬件检查输入的TCP数据包帧校验和
  #define CHECKSUM_CHECK_TCP              0
#else
  /* CHECKSUM_GEN_IP==1: Generate checksums in software for outgoing IP packets.*/
  #define CHECKSUM_GEN_IP                 1
  /* CHECKSUM_GEN_UDP==1: Generate checksums in software for outgoing UDP packets.*/
  #define CHECKSUM_GEN_UDP                1
  /* CHECKSUM_GEN_TCP==1: Generate checksums in software for outgoing TCP packets.*/
  #define CHECKSUM_GEN_TCP                1
  /* CHECKSUM_CHECK_IP==1: Check checksums in software for incoming IP packets.*/
  #define CHECKSUM_CHECK_IP               1
  /* CHECKSUM_CHECK_UDP==1: Check checksums in software for incoming UDP packets.*/
  #define CHECKSUM_CHECK_UDP              1
  /* CHECKSUM_CHECK_TCP==1: Check checksums in software for incoming TCP packets.*/
  #define CHECKSUM_CHECK_TCP              1
#endif

/*
   ----------------------------------------------
   ---------- SequentialAPI选项----------
   ----------------------------------------------
*/

//LWIP_NETCONN==1:使能NETCON函数(要求使用api_lib.c)
#define LWIP_TCPIP_CORE_LOCKING           0           //?
#define LWIP_NETCONN                      1           //应用层使用NETCONN相关函数
#define LWIP_SOCKET                     1	//LWIP_SOCKET==1:使能Sicket API(要求使用sockets.c)
#define LWIP_COMPAT_MUTEX               1
#define LWIP_SO_RCVTIMEO                1 	//通过定义LWIP_SO_RCVTIMEO使能netconn结构体中recv_timeout,使用recv_timeout可以避免阻塞线程


/*
   ------------------------------------
   ---------- Socket API选项----------
   ------------------------------------
*/
//LWIP_SOCKET==1:使能Sicket API(要求使用sockets.c)
//#define LWIP_SOCKET                       0               //关闭SOCKET通讯
//#define LWIP_COMPAT_SOCKETS             1
//#define LWIP_POSIX_SOCKETS_IO_NAMES     1
//#define LWIP_SO_RCVTIMEO                  0               //conn->acceptmbox/recvmbox接收有超时限制,超时后自动断开连接
//#define LWIP_SO_RCVBUF                  0
//#define SO_REUSE                        0
//#define LWIP_COMPAT_MUTEX               1


//*****************************************************************************
//
// ---------- TCP KEEPLIVE ----------
//
//*****************************************************************************
#define LWIP_TCP_KEEPALIVE                  1                //tcp保活定时器
#define TCP_KEEPIDLE_DEFAULT                60000            //保活时间 60s
#define TCP_KEEPINTVL_DEFAULT               10000            //保活探测 10s
#define TCP_KEEPCNT_DEFAULT                 9U  

//有关系统的选项
#define TCPIP_THREAD_STACKSIZE          1000				//*4	//内核任务堆栈大小
#define DEFAULT_UDP_RECVMBOX_SIZE       2000
#define DEFAULT_THREAD_STACKSIZE        512


//*****************************************************************************
//
// ---------- DEBUG_OPTIONS  ----------
//
//*****************************************************************************
#define DEBUG_OPTIONS                                   0                    //调试代码

/*
   ----------------------------------------
   ---------- Lwip调试选项----------
   ----------------------------------------
*/
//#define LWIP_DEBUG                     1 //开启DEBUG选项

#define ICMP_DEBUG                      LWIP_DBG_OFF //开启/关闭ICMPdebug

#endif /* __LWIPOPTS_H__ */

