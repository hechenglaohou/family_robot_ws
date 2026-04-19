from setuptools import find_packages
from setuptools import setup

setup(
    name='wechat_interfaces',
    version='0.0.0',
    packages=find_packages(
        include=('wechat_interfaces', 'wechat_interfaces.*')),
)
