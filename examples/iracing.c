#include "../src/iracing/client.c"

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    iracing_client_t client;

    iracing_client_init(&client);
    iracing_client_free(&client);

    return 0;
}
