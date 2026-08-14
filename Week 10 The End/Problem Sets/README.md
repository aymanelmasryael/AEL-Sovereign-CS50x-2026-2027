# 🚍 Week 10 — Final Project · AEL Sovereign Fleet Manager

> Original final project for **CS50x 2026-2027**, designed and engineered from scratch by **Ayman Elmasry — AEL Digital Studio**.

## 📋 Project Overview

A full-stack **public-transport fleet management system** built with Flask + SQLite — a production-style web application for managing vehicles, routes, ticketing, and admin oversight.

## ✨ Features

| Area | Details |
|------|---------|
| **Public Dashboard** | Live stats: fleet size, active routes, revenue, tickets sold |
| **Vehicle & Route Catalogue** | Browse the fleet board and schedule boards |
| **Customer Ticketing** | Search trips by origin/destination, buy tickets with live seat availability, query purchased tickets by name/phone |
| **Admin Suite** | Secure login, add/update/delete vehicles & routes, full ticket audit |
| **Engineering** | Parameterized SQL (no injection), Post-Redirect-Get on every POST, hashed admin credentials, flash feedback, apology error panels |

## 🛠 Run

```bash
cd "Week 10 The End/Problem Sets"
pip install -r requirements.txt
python3 app.py          # → http://127.0.0.1:5000
```

> Demo admin (seeded): `ael_admin` / `sovereign` (see app.py).

## 🗄 Data Model

`admins` · `vehicles` · `routes` · `tickets` — auto-created and seeded on first run.

---
**© Ayman Elmasry — AEL Digital Studio™ · All Rights Reserved.**