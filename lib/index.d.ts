export class CronetBuffer /* extends Buffer */ {
  initWithAlloc: (size: number) => void
}
export class CronetEngineParams {
  enableQuic: boolean
  enableHttp2: boolean
  disableCache: boolean
  userAgent: string
  experimentalOptions: string
}
export class CronetEngine {
  versionString: string
  startWithParams: (params: CronetEngineParams) => void
}
export class CronetError {
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
  disableCache: boolean
  httpMethod: string
  requestHeadersAdd: (header: CronetHttpHeader) => void
  requestHeadersAt: (i: number) => CronetHttpHeader
  requestHeadersClear: () => void
  requestHeadersSize: () => number
  uploadDataProviderExecutor: CronetExecutor
  uploadDataProviderProvider: CronetUploadDataProvider
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
