#include <libsparkproto/util.h>
#include <libsparkproto/configure.h>
#include <libsparkproto/constants.h>

namespace libspark {

namespace protocol {

void printLibraryInfo() {
    printf("\n");
    printf("Libsparkproto %s, protocol %d.%d.%d\n",
            LIBSPARKPROTO_VERSION, 
            PROTOCOL_VERSION[0],
            PROTOCOL_VERSION[1],
            PROTOCOL_VERSION[2]
    );
}

} // namespace protocol
} // namespace libspark
