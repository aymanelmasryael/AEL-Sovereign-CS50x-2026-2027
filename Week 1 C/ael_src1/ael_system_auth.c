/**
 * AEL Sovereign Systems — Custom CS50 Masterpiece (ael_src1)
 * Project: Enterprise Security Authentication & String Masking (ael_system_auth.c)
 * Description: Demonstrates strict command-line arguments (argc/argv), string parsing,
 *              basic ASCII obfuscation, and zero-trust conditional structures in bare C.
 */

#include <cs50.h>
#include <stdio.h>
#include <string.h>

bool authenticate_enterprise_key(string key);
void generate_access_token(string username);

int main(int argc, string argv[])
{
    // Ensure strict command-line usage protocol
    if (argc != 2)
    {
        printf("⚡ [AEL SECURITY PROTOCOL ERR]: Invalid arguments.\n");
        printf("Usage: ./ael_system_auth <MASTER_USER_NAME>\n");
        return 1;
    }

    string username = argv[1];
    printf("====================================================\n");
    printf("     AEL ENTERPRISE COGNITIVE TERMINAL v3.0\n");
    printf("====================================================\n");
    printf("Authenticating user profile: [%s]...\n", username);

    // Interactive Zero-Trust Prompt
    string passkey = get_string("Enter AEL Sovereign Core Passkey: ");

    if (authenticate_enterprise_key(passkey))
    {
        printf("\n🟢 [ACCESS GRANTED]: Sub-silicon matrix established.\n");
        generate_access_token(username);
        return 0;
    }
    else
    {
        printf("\n🔴 [ACCESS DENIED]: Unauthorized intrusion detected! Session locked.\n");
        return 2;
    }
}

/**
 * Validates the core enterprise key using strict length and char iteration
 */
bool authenticate_enterprise_key(string key)
{
    // Expected key: "AEL-COGNITIVE-2026"
    string master_key = "AEL-COGNITIVE-2026";
    
    if (strlen(key) != strlen(master_key))
    {
        return false;
    }

    for (int i = 0, n = strlen(key); i < n; i++)
    {
        if (key[i] != master_key[i])
        {
            return false;
        }
    }
    return true;
}

/**
 * Generates an obfuscated ASCII access token to showcase memory & loop math
 */
void generate_access_token(string username)
{
    printf("Generating secure token: ");
    for (int i = 0, n = strlen(username); i < n; i++)
    {
        // Shift ASCII value by +5 to demonstrate low-level char arithmetic
        printf("%c", username[i] + 5);
    }
    printf("-AEL-VERIFIED\n");
    printf("====================================================\n");
}
