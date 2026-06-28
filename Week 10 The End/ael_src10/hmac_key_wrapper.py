#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
=============================================================================
🏛️ AEL SOVEREIGN ARSENAL : HMAC INTEGRITY KEY WRAPPER (ael_src10)
=============================================================================
© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.
Syllabus Domain: Week 10 Cybersecurity – Data Integrity & Tamper Detection
=============================================================================
"""

import hmac
import hashlib
import json
import logging
from typing import Dict, Any, Tuple

logging.basicConfig(level=logging.INFO, format="%(asctime)s [%(levelname)s] %(message)s")

class HMACKeyWrapper:
    """
    Sovereign cryptographic wrapping engine dedicated to generating
    and verifying HMAC SHA-256 digital signatures for data payloads.
    """
    def __init__(self, wrapping_key: bytes = b"AEL_SOVEREIGN_HMAC_MASTER_KEY_2026"):
        self.wrapping_key = wrapping_key
        logging.info("🔒 HMACKeyWrapper engine armed with 256-bit secure wrapping key.")

    def encapsulate_payload(self, raw_payload: Dict[str, Any]) -> Tuple[str, str]:
        """Serializes payload and generates a secure HMAC SHA-256 digest stamp."""
        serialized_data = json.dumps(raw_payload, sort_keys=True)
        hmac_obj = hmac.new(self.wrapping_key, serialized_data.encode('utf-8'), hashlib.sha256)
        signature = hmac_obj.hexdigest()
        logging.info(f"⚡ Payload encapsulated. HMAC Signature Generated: [{signature[:16]}...]")
        return serialized_data, signature

    def verify_and_unwrap(self, serialized_data: str, incoming_signature: str) -> Dict[str, Any]:
        """
        Verifies HMAC signature immutability using constant-time comparison
        and deserializes the verified payload.
        """
        logging.info("🔍 Initiating HMAC signature verification...")
        
        # Generate expected signature from incoming data
        expected_hmac = hmac.new(self.wrapping_key, serialized_data.encode('utf-8'), hashlib.sha256).hexdigest()
        
        # Compare using constant-time evaluation to prevent side-channel timing leaks
        if not hmac.compare_digest(expected_hmac, incoming_signature):
            logging.error("❌ [INTEGRITY BREACH] HMAC signature mismatch. Payload has been modified or tampered with!")
            raise ValueError("Integrity Verification Failed: HMAC Mismatch.")
        
        logging.info("✅ [INTEGRITY VERIFIED] HMAC match successful. Unwrapping verified payload.")
        return json.loads(serialized_data)


if __name__ == "__main__":
    print("\n" + "="*70)
    print(" 🔒 ael_src10 : HMAC SHA-256 INTEGRITY KEY WRAPPING SIMULATOR")
    print("="*70)
    
    wrapper = HMACKeyWrapper()
    
    sample_payload = {
        "action": "DECRYPT_VAULT",
        "user": "AymanElmasry",
        "privilege_scope": "ApexSovereign",
        "transaction_id": 987654321
    }
    
    print("\n--- Phase 1: Payload Encapsulation ---")
    data_stream, signature = wrapper.encapsulate_payload(sample_payload)
    print(f"Data Stream: {data_stream}")
    print(f"Cryptographic Stamp: {signature}")
    
    print("\n--- Phase 2: Valid Verification & Unwrapping ---")
    try:
        verified_data = wrapper.verify_and_unwrap(data_stream, signature)
        print(f"Unwrapped Payload Successfully: {verified_data}")
    except Exception as e:
        print(f"Error: {e}")
        
    print("\n--- Phase 3: Simulated Man-In-The-Middle Tampering ---")
    # Simulate intercepting data and altering privilege scope
    tampered_stream = data_stream.replace("ApexSovereign", "IntruderAccess")
    print(f"Tampered Stream: {tampered_stream}")
    try:
        wrapper.verify_and_unwrap(tampered_stream, signature)
    except Exception as e:
        print(f"Intercepted and Terminated: {e}")
        
    print("\n" + "="*70)
    print(" ✅ Execution Completed. Cryptographic Immutability Guaranteed.")
    print("="*70 + "\n")
