export class CronetBuffer {
  initWithAlloc: (size: number) => void
  data: Buffer
  size: number
}
export class CronetEngineParams {
  static HTTP_CACHE_MODE: {
    DISABLED: number
    IN_MEMORY: number
    DISK_NO_HTTP: number
    DISK: number
  }
  enableQuic: boolean
  enableHttp2: boolean
  disableCache: boolean
  userAgent: string
  storagePath: string
  experimentalOptions: string
  httpCacheMode: number
  httpCacheMaxSize: number
}
export class CronetEngine {
  static loadLibrary: (filename: string) => void
  versionString: string
  getPtr: () => bigint
  startWithParams: (params: CronetEngineParams) => void
  startNetLogToFile: (filename: string, logAll: boolean) => boolean
  stopNetLog: () => void
  shutdown: () => void
}
export class CronetError {
  static ERROR_CODE: {
    ERROR_CALLBACK: number
    ERROR_HOSTNAME_NOT_RESOLVED: number
    ERROR_INTERNET_DISCONNECTED: number
    ERROR_NETWORK_CHANGED: number
    ERROR_TIMED_OUT: number
    ERROR_CONNECTION_CLOSED: number
    ERROR_CONNECTION_TIMED_OUT: number
    ERROR_CONNECTION_REFUSED: number
    ERROR_CONNECTION_RESET: number
    ERROR_ADDRESS_UNREACHABLE: number
    ERROR_QUIC_PROTOCOL_FAILED: number
    ERROR_OTHER: number
  }
  message: string
  errorCode: number
  internalErrorCode: number
}
export class CronetExecutor {
  start: () => void
  shutdown: () => void
}
export class CronetHttpHeader {
  name: string
  value: string
}
export class CronetUploadDataProvider {
  getLength: () => number
  read: (sink: CronetUploadDataSink, buffer: CronetBuffer) => void
  rewind: (sink: CronetUploadDataSink) => void
  close: () => void
}
export class CronetUploadDataSink {
  onReadError: (message: string) => void
  onReadSucceeded: (bytesRead: number, finalChunk: boolean) => void
  onRewindError: (message: string) => void
  onRewindSucceeded: () => void
}
export class CronetUrlRequestCallback {
  onRedirectReceived: (request: CronetUrlRequest, info: CronetUrlResponseInfo, newLocationUrl: string) => void
  onResponseStarted: (request: CronetUrlRequest, info: CronetUrlResponseInfo) => void
  onReadCompleted: (request: CronetUrlRequest, info: CronetUrlResponseInfo, buffer: CronetBuffer, bytesRead: number) => void
  onSucceeded: (request: CronetUrlRequest, info: CronetUrlResponseInfo) => void
  onFailed: (request: CronetUrlRequest, info: CronetUrlResponseInfo, error: CronetError) => void
  onCanceled: (request: CronetUrlRequest, info: CronetUrlResponseInfo) => void
}
export class CronetUrlRequestParams {
  static REQUEST_PRIORITY: {
    REQUEST_PRIORITY_IDLE: number
    REQUEST_PRIORITY_LOWEST: number
    REQUEST_PRIORITY_LOW: number
    REQUEST_PRIORITY_MEDIUM: number
    REQUEST_PRIORITY_HIGHEST: number
  }
  disableCache: boolean
  httpMethod: string
  requestHeadersAdd: (header: CronetHttpHeader) => void
  requestHeadersAt: (i: number) => CronetHttpHeader
  requestHeadersClear: () => void
  requestHeadersSize: () => number
  uploadDataProviderExecutor: CronetExecutor
  uploadDataProvider: CronetUploadDataProvider
}
export class CronetUrlRequest {
  initWithParams: (engine: CronetEngine, url: string, params: CronetUrlRequestParams, callback: CronetUrlRequestCallback, executor: CronetExecutor) => void
  start: () => void
  cancel: () => void
  read: (buffer: CronetBuffer) => void
  followRedirect: () => void
}
export class CronetUrlResponseInfo {
  httpStatusCode: number
  httpStatusText: string
  negotiatedProtocol: string
  url: string
  allHeadersListAdd: (header: CronetHttpHeader) => void
  allHeadersListAt: (i: number) => CronetHttpHeader
  allHeadersListClear: () => void
  allHeadersListSize: () => number
}
