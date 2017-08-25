//
//  EasyBlueToothManager.h
//  EasyBlueTooth
//
//  Created by nf on 2016/8/15.
//  Copyright © 2017年 chenSir. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "EasyCenterManager.h"
#import "EasyService.h"
#import "EasyCharacteristic.h"
#import "EasyPeripheral.h"
#import "EasyDescriptor.h"

#import "EasyManagerOptions.h"

/**
 * 连接一个设备所经历的状态
 */
typedef NS_ENUM(NSUInteger , bluetoothState) {
    
    bluetoothStateReadly = 0,     //蓝牙准备就绪
    bluetoothStateScanDevice ,  //扫描设备
    bluetoothStateConnect ,     //连接设备
    bluetoothStateService ,     //获取服务
    bluetoothStateCharacter ,   //获取特征
    bluetoothStateWriteData ,   //写数据
    bluetoothStateReceivedData ,//获取通知数据
    bluetoothStateDestory ,     //断开设备
    
};

typedef NS_ENUM(NSUInteger , bluetoothErrorState) {
    
    bluetoothErrorStateNoReadly = 0 ,//系统蓝牙没有打开
    bluetoothErrorStateNoDevice ,    //没有找到设备
    bluetoothErrorStateConnectError ,//连接失败
    bluetoothErrorStateDisconnect ,  //设备失去连接
    bluetoothErrorStateNoService ,   //没有找到相应的服务
    bluetoothErrorStateNoCharcter,  //没有对应的特征
    bluetoothErrorStatewriteError,  //写数据失败
    bluetoothErrorStateNotifyError ,//监听通知失败
};


/**
 * 模糊搜索设备规则
 * 用户可以自定义，依据peripheral里面的名称，广播数据，RSSI来赛选需要的连接的设备
 */
typedef BOOL (^blueToothScanRule)(EasyPeripheral *peripheral);

/**
 * 搜索到设备回调
 * peripheral 已经连接上的设备
 * error是扫描错误信息
 */
typedef void (^blueToothScanCallback)(EasyPeripheral *peripheral , NSError *error );


/**
 * 搜索到设备回调
 * deviceArray 里面是所有符合规则的设备。(需要处理peripheral里面的error信息)
 * error是扫描错误信息
 */
typedef void (^blueToothScanAllCallback)(NSArray<EasyPeripheral *> *deviceArray , NSError *error );


/**
 * 读写操作回调
 */
typedef void (^blueToothOperationCallback)(NSData *data , NSError *error);

@interface EasyBlueToothManager : NSObject


/**
 * 这个参数里面全都是放置的初始条件
 */
@property (nonatomic,strong)EasyManagerOptions *managerOptions ;

/**
 * 单例
 */
+ (instancetype)shareInstance ;


#pragma mark - 扫描并连接设备

/**
 * 连接一个已知名字的设备
 * name 设备名称
 * timeout 扫描设备 连接设备所使用的最长时间。
 * callback 连接设备的回调信息
 */
- (void)connectDeviceWithName:(NSString *)name
                      timeout:(NSInteger)timeout
                     callback:(blueToothScanCallback)callback ;

/**
 * 连接一个一定规则的设备，依据peripheral里面的名称，广播数据，RSSI来赛选需要的连接的设备
 * name 设备名称
 * timeout 扫描设备 连接设备所使用的最长时间。
 * callback 连接设备的回调信息
 */
- (void)connectDeviceWithRule:(blueToothScanRule)rule
                      timeout:(NSInteger)timeout
                     callback:(blueToothScanCallback)callback ;


/**
 * 连接一个确定ID的设备，一般此ID可以保存在本地。然后直接连接
 * name 设备名称
 * timeout 扫描设备 连接设备所使用的最长时间。
 * callback 连接设备的回调信息
 */
- (void)connectDeviceWithIdentifier:(NSString *)identifier
                            timeout:(NSInteger)timeout
                           callback:(blueToothScanCallback)callback ;


- (void)connectDeviceWithIdentifier:(NSString *)identifier
                            timeout:(NSInteger)timeout
                        connectOptions:(NSDictionary *)options
                           callback:(blueToothScanCallback)callback ;
/**
 * 一行代码连接所有的设备
 * name         一直设别的名称
 * timeout      连接超时时间
 * serviceuuid  服务id
 * notifyuuid   监听端口的id
 * writeuuid    写数据的id
 * data         需要发送给设备的数据
 * callback     回调信息
 */
- (void)connectDeviceWithName:(NSString *)name
                      timeout:(NSInteger)timeout
                  serviceUUID:(NSString *)serviceUUID
                   notifyUUID:(NSString *)notifyUUID
                    wirteUUID:(NSString *)writeUUID
                    writeData:(NSData *)data
                     callback:(blueToothScanCallback)callback;

/**
 * 连接已知名称的所有设备（返回的是一组此名称的设备全部连接成功）
 * name 设备名称
 * timeout 扫描设备 连接设备所使用的最长时间。
 * callback 连接设备的回调信息
 */
- (void)connectAllDeviceWithName:(NSString *)name
                         timeout:(NSInteger)timeout
                        callback:(blueToothScanAllCallback)callback ;

/**
 * 连接已知规则的全部设备（返回的是一组此名称的设备全部连接成功）
 * name 设备名称
 * timeout 扫描设备 连接设备所使用的最长时间。
 * callback 连接设备的回调信息
 */
- (void)connectAllDeviceWithRule:(blueToothScanRule)rule
                         timeout:(NSInteger)timeout
                        callback:(blueToothScanAllCallback)callback ;



#pragma mark - 读写操作

/**
 * peripheral 写数据的设备
 * data  需要写入的数据
 * uuid 数据需要写入到哪个特征下面
 * writeCallback 写入数据后的回调
 */
- (void)writeDataWithPeripheral:(EasyPeripheral *)peripheral
                           data:(NSData *)data
                      writeUUID:(NSString *)uuid
                       callback:(blueToothOperationCallback)callback ;

/**
 * peripheral 写数据的设备
 * uuid 需要读取数据的特征
 * writeCallback 读取数据后的回调
 */
- (void)readValueWithPeripheral:(EasyPeripheral *)peripheral
                        readUUID:(NSString *)uuid
                       callback:(blueToothOperationCallback)callback ;

/**
 * 建议此方法放在读写操作的前面
 */

/**
 * peripheral 写数据的设备
 * uuid 需要监听的特征值
 * writeCallback 读取数据后的回调
 */
- (void)notifyDataWithPeripheral:(EasyPeripheral *)peripheral
                      notifyUUID:(NSString *)uuid
                    withCallback:(blueToothOperationCallback )callback ;

/**
 * peripheral 写数据的设备
 * data  需要写入的数据
 * descroptor 需要往描述下写入数据
 * writeCallback 读取数据后的回调
 */
- (void)writeDescroptorWithPeripheral:(EasyPeripheral *)peripheral
                                 data:(NSData *)data
                           descroptor:(EasyDescriptor *)descroptor
                             callback:(blueToothOperationCallback)writeCallback ;

/**
 * peripheral 需要读取描述的设备
 * descroptor 需要往描述下写入数据
 * writeCallback 读取数据后的回调
 */
- (void)readDescroptorWithPeripheral:(EasyPeripheral *)peripheral
                          descroptor:(EasyDescriptor *)descroptor
                            callback:(blueToothOperationCallback)writeCallback ;

#pragma mark - 断开操作

/*
 * peripheral 需要断开的设备
// */
//- (void)disconnectWithPeripheral:(EasyPeripheral *)peripheral ;
//
///*
// * identifier 需要断开的设备UUID
// */
//- (void)disconnectWithIdentifier:(NSUUID *)identifier ;
//
///*
// * 断开所有连接的设备
// */
//- (void)disconnectAllPeripheral ;


#pragma mark - 简便方法
/*
 *
 *
 *
 *
 *
 */
//- (void)connectWithDeviceName:(NSString *)deviceName
//                 scanInterval:(NSTimeInterval)timeInterval
//                  serviceUUID:(NSString *)serviceUUID
//                    writeUUID:(NSString *)writeUUID
//                   notifyUUID:(NSString *)notifyUUID
//                    writeData:(NSData *)data
//          stateChangeCallback:(blueToothStateChangeCallback *)stateChangeCallback
//         receivedDataCallback:(blueToothOperationCallBack *)receivedDataCallback ;
@end

















