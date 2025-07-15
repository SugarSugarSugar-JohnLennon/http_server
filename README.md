# Muduo-like HTTP Server

这是一个用C++实现的高性能、muduo风格的多线程HTTP服务器，基于Reactor模式，支持静态文件服务、路由、GET/POST请求和简单日志。

## 特性
- Reactor事件驱动模型，one loop per thread
- epoll + 非阻塞IO
- 多线程（线程池）
- 静态文件服务（static目录）
- 路由机制，支持自定义API
- 支持GET/POST请求
- 简单日志输出

## 目录结构
```
http_server/
├── main.cpp
├── EventLoop.*
├── Channel.*
├── EpollPoller.*
├── Acceptor.*
├── ThreadPool.*
├── TcpServer.*
├── TcpConnection.*
├── HttpRequest.*
├── HttpResponse.*
├── utils.*
├── CMakeLists.txt
├── README.md
└── static/   # 静态文件目录
```

## 编译

```bash
mkdir build && cd build
cmake ..
make
```

## 运行

```bash
./http_server
```

默认监听8080端口。

## 路由与API示例

- 静态文件：访问 `http://localhost:8080/` 或 `http://localhost:8080/index.html`
- GET API：
  - `GET /api/hello` 返回 `{ "msg": "hello, world!" }`
- POST API：
  - `POST /api/echo`，请求体内容会被原样返回

## 日志

所有请求和错误会输出到标准输出，带时间戳。

## 扩展

你可以在 `main.cpp` 里通过 `registerRoute` 添加自定义路由和处理函数。

---

如需更多功能（如WebSocket、定时器、异步日志等），可在此基础上扩展。