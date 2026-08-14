#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
=============================================================================
🏛️ AEL SOVEREIGN ARSENAL : PROACTIVE PORT SCANNER GUARD (ael_src10)
=============================================================================
© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.
Syllabus Domain: Week 10 Cybersecurity – Network Reconnaissance & Hardening
=============================================================================
"""

import socket
import logging
import time
from typing import List, Tuple

logging.basicConfig(level=logging.INFO, format="%(asctime)s [%(levelname)s] %(message)s")

class PortScannerGuard:
    """
    Sovereign active edge scanning subsystem designed to perform proactive
    sweeps across listening sockets to detect unauthorized open ports.
    """
    def __init__(self, target_host: str = "127.0.0.1"):
        self.target_host = target_host
        self.authorized_ports = {80, 443, 22, 5000, 8000}
        logging.info(f"🌐 PortScannerGuard initialized for host [{target_host}].")
        logging.info(f"🛡️ Authorized System Ports: {self.authorized_ports}")

    def scan_ports(self, port_list: List[int]) -> List[Tuple[int, bool]]:
        """Scans a specified list of ports using fast socket connection probes."""
        logging.info(f"⚡ Initiating active socket sweep across {len(port_list)} ports...")
        scan_results = []
        
        for port in port_list:
            # Create a clean socket for each connection attempt
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            sock.settimeout(0.05) # Fast timeout for high-speed local sweeping
            try:
                result = sock.connect_ex((self.target_host, port))
                is_open = (result == 0)
                scan_results.append((port, is_open))
                if is_open:
                    if port in self.authorized_ports:
                        logging.info(f"🔵 Port [{port}] is OPEN (Authorized Service)")
                    else:
                        logging.warning(f"⚠️ [ROGUE PORT DETECTED] Port [{port}] is OPEN but UNAUTHORIZED!")
            except Exception as e:
                logging.debug(f"Socket error on port {port}: {e}")
            finally:
                sock.close()
                
        return scan_results

    def execute_security_audit(self, target_ports: List[int]) -> None:
        """Runs a complete network interface audit and compiles an apex defense report."""
        logging.info("===============================================================")
        logging.info(" 🛡️ STARTING SOVEREIGN APEX NETWORK SECURITY AUDIT")
        logging.info("===============================================================")
        start_time = time.time()
        
        results = self.scan_ports(target_ports)
        
        open_ports = [p for p, status in results if status]
        unauthorized = [p for p in open_ports if p not in self.authorized_ports]
        
        elapsed = time.time() - start_time
        logging.info("===============================================================")
        logging.info(f" ✅ Audit completed in {elapsed:.4f} seconds.")
        logging.info(f" 📊 Total Ports Scanned: {len(target_ports)}")
        logging.info(f" 🟢 Total Open Ports Found: {len(open_ports)} ({open_ports})")
        if unauthorized:
            logging.error(f" 🚨 WARNING: Unauthorized open ports detected: {unauthorized}. Edge firewall rule recommended!")
        else:
            logging.info(" 🛡️ Network perimeter fully secure. Zero unauthorized listening ports.")
        logging.info("===============================================================")


if __name__ == "__main__":
    print("\n" + "="*70)
    print(" 🌐 ael_src10 : PROACTIVE PORT SCANNER GUARD SIMULATION")
    print("="*70 + "\n")
    
    # Define sample port sweep matrix (authorized ports + common vulnerable ports)
    target_sweep_ports = [22, 23, 80, 443, 3306, 5000, 8000, 8080, 9000]
    
    guard = PortScannerGuard("127.0.0.1")
    guard.execute_security_audit(target_sweep_ports)
    
    print("\n" + "="*70)
    print(" ✅ Execution Completed. Network Reconnaissance Audit Finished.")
    print("="*70 + "\n")
