#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from std_msgs.msg import String
import requests
import json
from datetime import datetime

class StockMonitorNode(Node):
    def __init__(self):
        super().__init__('stock_monitor_node')
        
        # 1. 初始化发布者
        self.publisher_ = self.create_publisher(
            String, 
            '/wechat_message', 
            10
        )
        
        # 2. 配置监控参数
        self.stock_code = "03188"  # 监控的股票代码
        self.target_price = 66.0   # 目标价格
        self.check_interval = 60 * 60  # 检查间隔：60分钟（秒）
        
        # 3. 创建定时器
        self.timer = self.create_timer(
            self.check_interval, 
            self.check_stock_price
        )
        
        self.get_logger().info(f"股票监控节点已启动，开始监控{self.stock_code}，目标价格{self.target_price}元，检查间隔{self.check_interval/60}分钟")
        
        # 启动时立即检查一次
        self.check_stock_price()

    def get_stock_price(self):
        """获取港股03188的实时价格（使用腾讯财经接口）"""
        try:
            # 腾讯财经港股接口（稳定、无地域限制）
            # 参数说明：code=hk03188 对应股票代码03188
            url = f"https://qt.gtimg.cn/q=hk{self.stock_code}"
            headers = {
                'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36',
                'Referer': 'https://finance.qq.com/',
                'Accept': 'text/plain; charset=utf-8'
            }
            
            # 发送请求
            response = requests.get(url, headers=headers, timeout=15)
            response.encoding = 'gbk'  # 腾讯接口返回gbk编码
            raw_data = response.text.strip()
            
            # 解析腾讯财经的返回格式（示例：v_hk03188="...;25.8;..."）
            if not raw_data or '=' not in raw_data:
                self.get_logger().error(f"接口返回格式异常：{raw_data}")
                return None
            
            # 提取JSON部分并解析
            data_str = raw_data.split('="')[1].rstrip('";')
            data_list = data_str.split('~')
            
            # 腾讯港股接口字段：索引3是最新价格
            if len(data_list) < 4:
                self.get_logger().error(f"价格数据解析失败，返回数据：{data_list}")
                return None
            
            # 转换为浮点数
            latest_price = float(data_list[3])
            self.get_logger().info(f"{datetime.now()}: {self.stock_code} 当前价格：{latest_price}元")
            return latest_price
        
        except requests.exceptions.Timeout:
            self.get_logger().error("获取股票价格超时，请检查网络")
            return None
        except requests.exceptions.ConnectionError:
            self.get_logger().error("网络连接失败，请检查网络或接口地址")
            return None
        except ValueError:
            self.get_logger().error("价格转换为数字失败，请检查接口返回数据")
            return None
        except Exception as e:
            self.get_logger().error(f"获取股票价格失败：{str(e)}，原始返回：{response.text if 'response' in locals() else '无'}")
            return None

    def check_stock_price(self):
        """检查股票价格，达到目标则发布话题"""
        current_price = self.get_stock_price()
        
        if current_price and current_price >= self.target_price:
            msg = String()
            # 同时展示股票代码、当前价格和目标价格
            msg.data = f'{self.stock_code} 当前价格{current_price}元，达到目标价格{self.target_price}元'
            self.publisher_.publish(msg)
            self.get_logger().info(f"发布消息：{msg.data}")
        elif current_price:
            self.get_logger().info(f"{self.stock_code} 当前价格{current_price}元，未达到目标价格{self.target_price}元")
        else:
            self.get_logger().warn("未能获取有效价格，跳过本次检查")



def main(args=None):
    rclpy.init(args=args)
    node = StockMonitorNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        node.get_logger().info("节点被手动终止")
    finally:
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()
