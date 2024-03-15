const { Buffer } = require('node:buffer');
const cronetjs = require('../lib')
const {
  CronetBuffer,
  CronetHttpHeader,
  CronetEngineParams,
  CronetEngine,
  CronetExecutor,
  CronetUploadDataProvider,
  CronetUrlRequestCallback,
  CronetUrlRequestParams,
  CronetUrlRequest,
} = cronetjs

console.log(cronetjs)

CronetEngine.loadLibrary('libcronet.so')

let params = new CronetEngineParams()
let fields = []
for (const field in params) {
  fields.push(field)
}
console.log(params, 'fields:', fields)
params.enableQuic = false
params.userAgent = 'CronetJS/1'
params.experimentalOptions = '{"QUIC":{}}'
console.log({
  enableQuic: params.enableQuic,
  enableHttp2: params.enableHttp2,
  enableBrotli: params.enableBrotli,
  userAgent: params.userAgent,
  experimentalOptions: params.experimentalOptions,
})

const engine = new CronetEngine()
fields = []
for (const field in engine) {
  fields.push(field)
}
console.log(engine, 'fields:', fields)
console.log('version: ' + engine.versionString)

engine.startWithParams(params)
params = null

let buffer = new CronetBuffer()
try {
  buffer.initWithAlloc('123')
} catch (e) {
  console.error(e)
}

const executor = new CronetExecutor()
fields = []
for (const field in executor) {
  fields.push(field)
}
console.log(executor, 'fields:', fields)

executor.start()

function finish() {
  setTimeout(function () {
    console.log('Shutting down...')
    executor.shutdown()
    engine.shutdown()
  }, 0)
}

function newRequest(method, url, opts = null) {
  return new Promise((resolve, reject) => {
    let urlReqParams = new CronetUrlRequestParams()
    fields = []
    for (const field in urlReqParams) {
      fields.push(field)
    }
    console.log(urlReqParams, 'fields:', fields)
    urlReqParams.httpMethod = method
    urlReqParams.disableCache = true
    if (opts && opts.priority) {
      urlReqParams.priority = CronetUrlRequestParams.REQUEST_PRIORITY.REQUEST_PRIORITY_HIGHEST
    }
    console.log({
      httpMethod: urlReqParams.httpMethod,
      disableCache: urlReqParams.disableCache,
      priority: urlReqParams.priority,
    })

    let reqHeader = new CronetHttpHeader()
    fields = []
    for (const field in reqHeader) {
      fields.push(field)
    }
    console.log(reqHeader, 'fields:', fields)
    reqHeader.name = 'X-Test'
    reqHeader.value = '1'
    urlReqParams.requestHeadersAdd(reqHeader)

    if (method === 'POST') {
      let uploadDataProvider = new CronetUploadDataProvider()
      fields = []
      for (const field in uploadDataProvider) {
        fields.push(field)
      }
      console.log(uploadDataProvider, 'fields:', fields)
      uploadDataProvider.getLength = function () {
        console.log('uploadDataProvider getLength')
        return 4
      }
      uploadDataProvider.read = function (sink, buffer) {
        const fields = []
        for (const field in sink) {
          fields.push(field)
        }
        console.log(sink, 'fields:', fields)
        console.log('uploadDataProvider read, buffer size: ' + buffer.size)
        const buf = Buffer.alloc(4, 'test')
        buf.copy(buffer.data, 0)
        console.log(buffer.data.slice(0, buf.length))
        sink.onReadSucceeded(buf.length, false)
      }
      uploadDataProvider.rewind = function (sink) {
        console.log('uploadDataProvider rewind')
        sink.onRewindSucceeded()
      }
      uploadDataProvider.close = function () {
        console.log('uploadDataProvider close')
      }
      urlReqParams.uploadDataProviderExecutor = executor
      urlReqParams.uploadDataProvider = uploadDataProvider
      console.log('uploadDataProviderExecutor same?', urlReqParams.uploadDataProviderExecutor === executor)
      console.log('uploadDataProvider same?', urlReqParams.uploadDataProvider === uploadDataProvider)
    }

    let urlCallback = new CronetUrlRequestCallback()
    fields = []
    for (const field in urlCallback) {
      fields.push(field)
    }
    console.log(urlCallback, 'fields:', fields)

    let body = null

    let urlReq = new CronetUrlRequest()
    fields = []
    for (const field in urlReq) {
      fields.push(field)
    }
    console.log(urlReq, 'fields:', fields)

    let timeout = setTimeout(function () {
      console.log('Request timeout.')
      timeout = null
      urlReq.cancel()
    }, (opts && opts.timeout) || 2000)

    const done = function () {
      if (timeout) {
        clearTimeout(timeout)
        timeout = null
      }
      if (body) {
        // console.log('response body:', body.toString())
      }
      urlReq = null
      urlCallback = null
      resolve()
    }

    urlCallback.onRedirectReceived = function (request, info, newLocationUrl) {
      console.log('onFollowRedirect: ' + info.httpStatusCode + ' ' + newLocationUrl)
      request.followRedirect()
    }
    urlCallback.onResponseStarted = function (request, info) {
      console.log('onResponseStarted: ' + info.url + ' ' + info.negotiatedProtocol + ' ' + info.httpStatusCode + ' ' + info.httpStatusText)
      let fields = []
      for (const field in info) {
        fields.push(field)
      }
      console.log(info, fields)
      console.log('All headers:')
      const allHeadersListSize = info.allHeadersListSize()
      for (let i = 0; i < allHeadersListSize; i++) {
        const header = info.allHeadersListAt(i)
        console.log({ name: header.name, value: header.value })
      }
      let buffer = new CronetBuffer()
      // Create and allocate 32kb buffer.
      buffer.initWithAlloc(32 * 1024)
      // Started reading the response.
      request.read(buffer)
    }
    urlCallback.onReadCompleted = function (request, info, buffer, bytesRead) {
      console.log('onReadCompleted, bytes read: ' + bytesRead)
      const fields = []
      for (const field in buffer) {
        fields.push(field)
      }
      const bytes = buffer.data.slice(0, bytesRead)
      console.log(buffer, fields, 'size: ' + buffer.size + ', bytes:', bytes)
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
      console.log('onFailed:', error)
      console.log('message: ' + error.message + ', error_code: ' + error.errorCode + ', internal_error_code: ' + error.internalErrorCode)
      done()
    }
    urlCallback.onCanceled = function (request, info) {
      console.log('onCanceled')
      done()
    }

    urlReq.initWithParams(engine, url, urlReqParams, urlCallback, executor)
    urlReqParams = null
    console.log('gc now')
    global.gc()
    if (opts && opts.noStart) {
      done()
    } else {
      setTimeout(function () {
        urlReq.start()
      }, 500)
    }
  })
}

function timeout(ms) {
  return new Promise((resolve, reject) => {
    setTimeout(resolve, ms)
  })
}

async function main() {
  await newRequest('POST', 'https://nodejs.org/echo')
  await newRequest('GET', 'https://www.google.com/', { priority: true })
  console.log('gc now')
  global.gc()
  await timeout(2000)
  await Promise.all([
    newRequest('GET', 'https://www.youtube.com/'),
    newRequest('GET', 'https://www.facebook.com/'),
  ])
  // no start
  await newRequest('GET', 'https://nodejs.org/', { noStart: true })
  global.gc()
  await timeout(500)
  finish()
}

main()
