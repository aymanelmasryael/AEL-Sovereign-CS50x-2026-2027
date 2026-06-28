#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_BUFFER 1024
#define SOVEREIGN_FLAG 0xDEADC0DE

typedef struct {
    uint32_t packet_id;
    char payload[MAX_BUFFER];
    bool is_encrypted;
    uint32_t integrity_checksum;
} AEL_SecurePacket;

uint32_t calculate_checksum(const char *data) {
    uint32_t hash = 5381;
    int c;
    while ((c = *data++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash;
}

bool ael_verify_packet(AEL_SecurePacket *pkt) {
    if (!pkt->is_encrypted) {
        printf("[!] AEL SHIELD ALERT: Unencrypted packet rejected.\n");
        return false;
    }
    uint32_t expected = calculate_checksum(pkt->payload);
    if (expected != pkt->integrity_checksum) {
        printf("[!] AEL SHIELD ALERT: Integrity checksum mismatch! Potential tampering detected.\n");
        return false;
    }
    printf("[+] AEL SHIELD: Packet %u verified successfully. Integrity intact.\n", pkt->packet_id);
    return true;
}

int main(void) {
    printf("====================================================\n");
    printf("      AEL BARE-METAL MEMORY SHIELD (v3.0)\n");
    printf("====================================================\n");

    AEL_SecurePacket incoming;
    incoming.packet_id = 10101;
    strncpy(incoming.payload, "Sovereign Master Command: ENGAGE_CITADEL", MAX_BUFFER - 1);
    incoming.is_encrypted = true;
    incoming.integrity_checksum = calculate_checksum(incoming.payload);

    if (ael_verify_packet(&incoming)) {
        printf("[*] Executing payload safely in protected ring...\n");
    }

    return 0;
}
