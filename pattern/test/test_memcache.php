<?php

$memcacheObj = new Memcached;

$memcacheObj->addServer('127.0.0.1', 10111);
$memcacheObj->setOption(Memcached::OPT_BINARY_PROTOCOL, true);
$memcacheObj->setOption(Memcached::OPT_COMPRESSION, false);
$memcacheObj->setOption(\Memcached::OPT_POLL_TIMEOUT, 30);
$memcacheObj->setOption(\Memcached::OPT_RECV_TIMEOUT, 30);
$memcacheObj->setOption(\Memcached::OPT_SEND_TIMEOUT, 30);

$key = '{"dict": "dicta","contents": ["测试"]}';

var_dump($memcacheObj->get(base64_encode($key)));
