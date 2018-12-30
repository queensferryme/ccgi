# CCGI - Mini FastCGI Written in C

## Introduction

**CCGI** is a mini fast-cgi implementation written in C. It is a piece of experimental workout & can not be deployed in production environment. However, this repository may be appeal to you if you are interested in following subjects:

- fundamental structure of CGI programs
- distinctions between CGI and FastCGI
- sqlite3 API implementation in C

## Setup

### Dependency

*(under Ubuntu/Debian)*

- spawn-fcgi `apt-get install spawn-fcig`
- fcgi `apt-get install libfcgi-dev`
- sqlite3 `apt-get install libsqlite3-dev`

### Build

```shell
git clone https://github.com/queensferryme/ccgi
cd ccgi & make init
```

If you see something like `spawn-fcgi: child spawned successfully: PID: <pid>` , then FastCGI progress is successfully spawned.

## Usage

*(following examples use`curl`)*

*(this is a RESTful api for managing student scores)*

**Note: You can't send HTTP requests to fast-cgi directly. You need a server(Apache/Nginx) as an inter-media.**

- GET /user?id=id&name=name

  ```shell
  curl "127.0.0.1:4000/user?id=2018211000&name=JackSon"
  # {"count":1,"result":{"id":"2018211000","name":"JackSon","math":100,"physics":100,"language":100}
  ```

- GET /admin

  ```shell
  curl "127.0.0.1:4000/admin"
  # {"result":[{"id":"2018211000","name":"JackSon","math":100,"physics":100,"language":100},...],"count":5}
  ```

- POST /update?id=id?name=name

  ```shell
  curl -d "math=80&physics=90&language=100" -X POST "127.0.0.1:4000/update?id=2018211000&name=JackSon"
  # {"status":"success"}
  ```

- GET /delete?id=id&name=name

  ```shell
  curl "127.0.0.1:4000/delete?id=2018211000&name=JackSon"
  # {"status":"success"}
  ```


## About

This project is part of the final project of *C Programming Language* & I have a [blog post](https://queensferry.coding.me/2018/12/30/%E5%88%9D%E6%AD%A5%E8%AE%A4%E8%AF%86CGI-FastCGI%EF%BC%9A%E5%9F%BA%E6%9C%AC%E6%9E%B6%E6%9E%84%E4%B8%8EC%E8%AF%AD%E8%A8%80%E5%AE%9E%E7%8E%B0/) to explain in detail.