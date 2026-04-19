import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Image
from cv_bridge import CvBridge
import cv2

class ImagePublisher(Node):
    def __init__(self):
        super().__init__('image_publisher')
        self.publisher_ = self.create_publisher(Image, '/image_raw', 10)
        self.bridge = CvBridge()
        self.image_path = "/home/hhc/sy_arduino/learn_yolo/zidane.jpg"
        self.image = cv2.imread(self.image_path)
        if self.image is None:
            self.get_logger().error(f"无法读取图片：{self.image_path}")
            return
        self.timer = self.create_timer(1.0, self.timer_callback)
        self.get_logger().info(f"开始发布图片话题 /image_raw")

    def timer_callback(self):
        ros_image = self.bridge.cv2_to_imgmsg(self.image, encoding="bgr8")
        self.publisher_.publish(ros_image)

def main(args=None):
    rclpy.init(args=args)
    node = ImagePublisher()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == "__main__":
    main()
