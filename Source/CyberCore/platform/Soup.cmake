list(APPEND CyberCore_PRIVATE_INCLUDE_DIRECTORIES
    "${WEBCORE_DIR}/platform/network/soup"
)

list(APPEND CyberCore_UNIFIED_SOURCE_LIST_FILES
    "platform/SourcesSoup.txt"
)

list(APPEND CyberCore_PRIVATE_FRAMEWORK_HEADERS
    platform/network/soup/AuthenticationChallenge.h
    platform/network/soup/CertificateInfo.h
    platform/network/soup/DNSResolveQueueSoup.h
    platform/network/soup/GRefPtrSoup.h
    platform/network/soup/GUniquePtrSoup.h
    platform/network/soup/ResourceError.h
    platform/network/soup/ResourceRequest.h
    platform/network/soup/ResourceResponse.h
    platform/network/soup/SocketStreamHandleImpl.h
    platform/network/soup/SoupNetworkProxySettings.h
    platform/network/soup/SoupNetworkSession.h
    platform/network/soup/URLSoup.h
    platform/network/soup/CyberKitSoupRequestGeneric.h
    platform/network/soup/CyberKitSoupRequestGenericClient.h
)

list(APPEND CyberCore_SYSTEM_INCLUDE_DIRECTORIES
    ${LIBSOUP_INCLUDE_DIRS}
)

list(APPEND CyberCore_LIBRARIES
    ${LIBSOUP_LIBRARIES}
)
