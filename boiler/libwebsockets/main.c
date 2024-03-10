// get an env var from a host (you don't want to do this in prod)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h> // getenv

#include <libwebsockets.h>

struct SessionInfo
{
	char sessionId[33];
};

static int callback_http(struct lws *wsi,
                         enum lws_callback_reasons reason,
                         void *user,
                         void *in, size_t len)
{
    return 0;
}

static int callback_bla(struct lws *wsi,
                        enum lws_callback_reasons reason,
                        void *user,
                        void *in, size_t len)
{
    struct SessionInfo *session = (struct SessionInfo *)user;

    switch (reason)
    {
    case LWS_CALLBACK_ESTABLISHED:
        // reset session info
        memset(session->sessionId, 0, 33);

        // generate new session id
        unsigned long secs = lws_now_secs();
        snprintf(session->sessionId, 33, "%ld", secs);

        // log new session
        printf("new session : %s\n", session->sessionId);

        break;
    case LWS_CALLBACK_RECEIVE:
    {
        // get session info
        printf("received data through session '%s'\n", session->sessionId);

        // get env var val
        char * val = getenv((char *)in);

        // if you need your user-instance (eg.: to call a cpp instance method), here we just stored argv[0]
        char * argv = lws_context_user(lws_get_context(wsi));
        printf("argv[0] : %s\n", argv);

        if (val != NULL)
        {
            // write answer
            size_t vallen = strlen(val);
            unsigned char *buf = (unsigned char*) malloc(LWS_SEND_BUFFER_PRE_PADDING + vallen + LWS_SEND_BUFFER_POST_PADDING);
            memcpy(buf+LWS_SEND_BUFFER_PRE_PADDING, val, vallen);
            lws_write(wsi, &buf[LWS_SEND_BUFFER_PRE_PADDING], vallen, LWS_WRITE_TEXT);
            free(buf);
        }

        break;
    }
    default:
        break;
    }
   
    return 0;
}

static struct lws_protocols protocols[] = {
    /* first protocol must always be HTTP handler */
    {
        "http-only",         // name
        callback_http,       // callback
        0                    // per_session_data_size
    },
    {
        "bla",               // protocol name - very important!
        callback_bla,        // callback
        sizeof(struct SessionInfo)  // per_session_data_size

    },
    {
        NULL, NULL, 0   /* End of list */
    }
};

int main(int argc, char *argv[]) {
    // server url will be http://localhost:9000
    int port = 9000;
    const char *interface = NULL;
    struct lws_context *context;
    // we're not using ssl
    const char *cert_path = NULL;
    const char *key_path = NULL;
    // no special options
    int opts = 0;
   
    // create libwebsocket context representing this server
    struct lws_context_creation_info info;

    memset(&info, 0, sizeof info);
    info.port = port;
    info.iface = interface;
    info.protocols = protocols;
    info.extensions = lws_get_internal_extensions();
    //if (!use_ssl) {
    info.ssl_cert_filepath = NULL;
    info.ssl_private_key_filepath = NULL;
    //} else {
    // info.ssl_cert_filepath = LOCAL_RESOURCE_PATH"/libwebsockets-test-server.pem";
    // info.ssl_private_key_filepath = LOCAL_RESOURCE_PATH"/libwebsockets-test-server.key.pem";
    //}
    info.gid = -1;
    info.uid = -1;
    info.options = opts;
    info.user = argv[0];

    context = lws_create_context(&info);
   
    if (context == NULL)
    {
        fprintf(stderr, "libwebsocket init failed\n");
        return -1;
    }
   
    printf("starting server...\n");
   
    // infinite loop, to end this server send SIGTERM. (CTRL+C)
    while (1) {
        lws_service(context, 50);
        // lws_service will process all waiting events with their
        // callback functions and then wait 50 ms.
        // (this is a single threaded webserver and this will keep our server
        // from generating load while there are not requests to process)
    }
   
    lws_context_destroy(context);
   
    return 0;
}


