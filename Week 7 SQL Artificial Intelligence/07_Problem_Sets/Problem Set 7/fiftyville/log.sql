-- ==============================================================================
-- Project   : AEL Sovereign — CS50x 2026-2027
-- Module    : week7_fiftyville_pset7
-- File      : log.sql
-- Author    : Ayman Elmasry — AEL Digital Studio
-- ------------------------------------------------------------------------------
-- Description:
--   INVESTIGATION JOURNAL — The Case of the Stolen CS50 Duck (Fiftyville).
--   This file is a complete, reproducible forensic narrative. Every SQL query
--   executed during the investigation is preserved with a comment explaining
--   WHY it was run, WHAT it revealed, and HOW it narrowed the suspect pool.
--   The investigation converges on a single thief, their escape route, and
--   their accomplice.
--
-- Investigation design (deductive narrowing):
--   1. Establish the crime   — read the police report for date/place.
--   2. Collect witness leads — three interviews name the bakery, the ATM on
--      Leggett Street, and a <1-minute phone call about an early flight.
--   3. Correlate hard facts  — each lead is turned into a concrete table
--      query (bakery exits 10:15-10:25, Leggett Street withdrawals, short
--      calls, earliest morning flight).
--   4. Intersect suspects    — the thief must satisfy ALL leads at once.
--      Each clue is an independent data source; their intersection collapses
--      the suspect set to one person.
--   5. Trace the escape      — identify the destination airport and the
--      accomplice who booked the ticket.
--
-- Final conclusion (verified against the database):
--   THIEF     : Bruce
--   ESCAPED TO: New York City (flight #36, dep. 08:20 on 29 July)
--   ACCOMPLICE: Robin (purchased the ticket; took Bruce's phone call)
-- ==============================================================================

-- ------------------------------------------------------------------------------
-- STEP 1 — Read the police report.
-- We know the theft took place in Humphrey Street; we must confirm the day
-- and extract the exact description of the crime.
-- ------------------------------------------------------------------------------
SELECT description
FROM crime_scene_reports
WHERE month = 7 AND day = 28 AND street = "Humphrey Street";

-- The report states: the CS50 duck was stolen at 10:15am at the Humphrey
-- Street bakery, and that THREE witnesses gave interviews on the same day.

-- ------------------------------------------------------------------------------
-- STEP 2 — Interview the witnesses.
-- All three interviews were conducted on 28 July; each transcript may name a
-- distinct piece of evidence (parking-lot getaway car, ATM withdrawal, call).
-- ------------------------------------------------------------------------------
SELECT name, transcript
FROM interviews
WHERE month = 7 AND day = 28;

-- Witness 1 (Ruth) : saw the thief get into a car in the bakery parking lot
--                     and drive away within ten minutes of the theft.
-- Witness 2 (Eugene): recognised the thief withdrawing money from an ATM on
--                     Leggett Street earlier that morning.
-- Witness 3 (Raymond): as the thief left the bakery, they called someone for
--                     less than a minute and mentioned taking the EARLIEST
--                     flight out of Fiftyville the next day, asking the other
--                     party to purchase the ticket.

-- ------------------------------------------------------------------------------
-- STEP 3 — Lead 1: Cars leaving the bakery parking lot (10:15 - 10:25).
-- The getaway window is the ten minutes after 10:15. Exit events in that
-- window produce candidate licence plates.
-- ------------------------------------------------------------------------------
SELECT license_plate
FROM bakery_security_logs
WHERE month = 7 AND day = 28
  AND hour = 10 AND minute BETWEEN 15 AND 25
  AND activity = "exit";

-- 8 plates: 5P2BI95, 94KL13X, 6P58WS2, 4328GD8, G412CB7, L93JTIZ, 322W7JE, 0NTHK55

-- ------------------------------------------------------------------------------
-- STEP 4 — Lead 2: ATM withdrawals on Leggett Street.
-- Eugene saw the thief at the ATM; we pull every Leggett Street withdrawal
-- on the morning of the 28th, then resolve each account to its owner.
-- ------------------------------------------------------------------------------
SELECT people.name, atm_transactions.account_number
FROM atm_transactions
JOIN bank_accounts ON bank_accounts.account_number = atm_transactions.account_number
JOIN people ON people.id = bank_accounts.person_id
WHERE month = 7 AND day = 28
  AND atm_location = "Leggett Street"
  AND transaction_type = "withdraw";

-- 8 names: Bruce, Diana, Brooke, Kenny, Iman, Luca, Taylor, Benista.

-- ------------------------------------------------------------------------------
-- STEP 5 — Lead 3: Short phone calls (< 1 minute) on the 28th.
-- Raymond overheard a <1-minute call as the thief left the bakery. List the
-- callers, and resolve them to names, to find the person who called someone.
-- ------------------------------------------------------------------------------
SELECT people.name AS caller, phone_calls.receiver
FROM phone_calls
JOIN people ON people.phone_number = phone_calls.caller
WHERE month = 7 AND day = 28 AND duration < 60;

-- 9 calls. Suspect callers include Sofia, Kelsey, Bruce, Taylor, Diana, Kenny.

-- ------------------------------------------------------------------------------
-- STEP 6 — Lead 4: The earliest flight out of Fiftyville on 29 July.
-- Raymond heard the thief say they planned to take the EARLIEST flight the
-- next day. First confirm which airport serves Fiftyville, then list flights.
-- ------------------------------------------------------------------------------
SELECT id, abbreviation, full_name, city
FROM airports
WHERE city = "Fiftyville";

-- Fiftyville Regional Airport has id = 8.

SELECT id, destination_airport_id, hour, minute
FROM flights
WHERE month = 7 AND day = 29 AND origin_airport_id = 8
ORDER BY hour, minute;

-- The earliest flight is #36, departing 08:20, destination airport id = 4.
-- Next, resolve airport 4 to find the escape destination.
SELECT full_name, city
FROM airports
WHERE id = 4;

-- Airport 4 = LaGuardia Airport in NEW YORK CITY.

-- ------------------------------------------------------------------------------
-- STEP 7 — Intersect the leads to isolate the thief.
-- The suspect must satisfy ALL FOUR constraints simultaneously:
--   (a) parked/bakery exit within 10:15-10:25,
--   (b) Leggett Street ATM withdrawal on the 28th,
--   (c) caller of a <1-minute call on the 28th,
--   (d) passenger on the earliest 29 July flight (id 36).
-- Each table is joined on its natural foreign key (people ↔ license_plate,
-- bank_accounts ↔ account, phone_calls ↔ caller, passengers ↔ passport).
-- ------------------------------------------------------------------------------
SELECT DISTINCT p.name, p.passport_number, p.license_plate
FROM people AS p
JOIN bakery_security_logs AS bsl ON bsl.license_plate = p.license_plate
JOIN bank_accounts AS ba ON ba.person_id = p.id
JOIN atm_transactions AS atm ON atm.account_number = ba.account_number
JOIN phone_calls AS pc ON pc.caller = p.phone_number
JOIN passengers AS ps ON ps.passport_number = p.passport_number
JOIN flights AS f ON f.id = ps.flight_id
WHERE bsl.month = 7 AND bsl.day = 28
  AND bsl.hour = 10 AND bsl.minute BETWEEN 15 AND 25
  AND bsl.activity = "exit"
  AND atm.month = 7 AND atm.day = 28
  AND atm.atm_location = "Leggett Street"
  AND atm.transaction_type = "withdraw"
  AND pc.month = 7 AND pc.day = 28 AND pc.duration < 60
  AND f.id = 36;

-- EXACTLY ONE row survives the intersection: BRUCE
-- (passport 5773159633, licence plate 94KL13X).
-- Every alternative candidate fails at least one clue, so Bruce is the thief.

-- ------------------------------------------------------------------------------
-- STEP 8 — Confirm the accomplice.
-- Raymond said the thief ASKED someone to buy the ticket and called them for
-- under a minute. Find who received Bruce's short call on the 28th; that
-- person arranged the flight and is the accomplice.
-- ------------------------------------------------------------------------------
SELECT people.name AS accomplice
FROM phone_calls
JOIN people ON people.phone_number = phone_calls.receiver
WHERE month = 7 AND day = 28 AND duration < 60
  AND caller = (SELECT phone_number FROM people WHERE name = "Bruce");

-- The receiver of Bruce's short call is ROBIN.

-- ------------------------------------------------------------------------------
-- FINAL VERDICT
-- ------------------------------------------------------------------------------
-- THIEF      : Bruce
-- ESCAPED TO : New York City (flight #36, 08:20, LaGuardia Airport)
-- ACCOMPLICE : Robin (booked the flight and received the short call)
-- ==============================================================================
