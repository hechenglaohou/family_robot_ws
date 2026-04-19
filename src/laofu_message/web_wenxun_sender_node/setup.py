from setuptools import find_packages, setup

package_name = 'web_wenxun_sender_node'

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
    maintainer_email='user@100ask.com',
    description='TODO: Package description',
    license='Apache-2.0',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'web_wenxun_sender=web_wenxun_sender_node.web_wenxun_sender:main'
        ],
    },
)
