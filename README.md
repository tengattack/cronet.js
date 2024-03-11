# Cronet.js

[Cronet](https://chromium.googlesource.com/chromium/src/+/refs/heads/main/components/cronet)'s native API bindings for Node.js.

## Usage

```js
const { Buffer } = require('node:buffer')
const cronetjs = require('./build/Release/cronetjs.node')
const {
  CronetEngineParams,
  CronetEngine,
  CronetExecutor,
  CronetUrlRequestCallback,
  CronetUrlRequestParams,
  CronetUrlRequest,
} = cronetjs

const params = new CronetEngineParams()
params.enableQuic = true // by default
const engine = new CronetEngine()
console.log('CronetEngine version: ' + engine.versionString)
engine.startWithParams(params)

const executor = new CronetExecutor()
executor.start()

function newRequest(url, timeout = 2000) {
  return new Promise((resolve, reject) => {
    const urlReq = new CronetUrlRequest()

    let body = null

    let t = setTimeout(function () {
      if (!t) {
        return
      }
      console.log('Request timeout.')
      t = null
      urlReq.cancel()
    }, timeout)

    const done = function () {
      if (t) {
        clearTimeout(t)
        t = null
      }
      resolve(body)
    }

    const urlReqParams = new CronetUrlRequestParams()
    urlReqParams.httpMethod = 'GET'
    urlReqParams.disableCache = true

    const urlCallback = new CronetUrlRequestCallback()
    urlCallback.onRedirectReceived = function (request, info, newLocationUrl) {
      console.log('onFollowRedirect: ' + info.httpStatusCode + ' ' + newLocationUrl)
      request.followRedirect()
    }
    urlCallback.onResponseStarted = function (request, info) {
      console.log('onResponseStarted: ' + info.url
        + ' ' + info.negotiatedProtocol
        + ' ' + info.httpStatusCode + ' ' + info.httpStatusText)

      console.log('All headers:')
      const allHeadersListSize = info.allHeadersListSize()
      for (let i = 0; i < allHeadersListSize; i++) {
        const header = info.allHeadersListAt(i)
        console.log({ name: header.name, value: header.value })
      }

      let buffer = new cronetjs.CronetBuffer()
      // Create and allocate 32kb buffer.
      buffer.initWithAlloc(32 * 1024)
      // Started reading the response.
      request.read(buffer)
    }
    urlCallback.onReadCompleted = function (request, info, buffer, bytesRead) {
      console.log('onReadCompleted, bytes read: ' + bytesRead)
      const bytes = buffer.data.slice(0, bytesRead)
      if (!body) {
        body = Buffer.from(bytes)
      } else {
        body = Buffer.concat([body, bytes])
      }
      // Continue reading the response.
      request.read(buffer)
    }
    urlCallback.onSucceeded = function (request, info) {
      console.log('onSucceeded')
      done()
    }
    urlCallback.onFailed = function (request, info, error) {
      console.log('onFailed, message: ' + error.message
        + ', error_code: ' + error.errorCode
        + ', internal_error_code: ' + error.internalErrorCode)
      done()
    }
    urlCallback.onCanceled = function (request, info) {
      console.log('onCanceled')
      done()
    }

    urlReq.initWithParams(engine, url, urlReqParams, urlCallback, executor)
    urlReq.start()
  })
}

async function main() {
  await newRequest('https://google.com/', 2000)
  executor.shutdown()
  engine.shutdown()
}

main()
```
