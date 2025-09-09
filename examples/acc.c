#include "../src/acc.c"

void check_error(bool has_error, char *operation) {
    if (has_error == true) {
        fprintf(stderr, "%s(...) failed with error %d\n", operation, acc_get_last_error());
        exit(-1);
    }
}

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    acc_client_t *client = acc_client_create("0.0.0.0", 1234);
    check_error(client == NULL, "acc_client_create");

    return 0;
}
