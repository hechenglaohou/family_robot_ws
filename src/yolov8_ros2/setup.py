from setuptools import find_packages, setup

package_name = 'yolov8_ros2'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='hhc',
    maintainer_email='hhc@todo.todo',
    description='TODO: Package description',
    license='Apache-2.0',
    extras_require={
        'test': [
            'pytest',
        ],
    },
    entry_points={
        'console_scripts': [
        "yolov8_ros2=yolov8_ros2.yolov8_ros2:main",
        "image_publisher = yolov8_ros2.publish_image:main"
        ],
    },
)
