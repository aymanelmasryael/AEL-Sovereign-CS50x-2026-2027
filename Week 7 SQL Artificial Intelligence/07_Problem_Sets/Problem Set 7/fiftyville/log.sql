-- ============================================================
-- AEL Sovereign — CS50x 2026-2027
-- Problem Set 7: Fiftyville
-- Author: Ayman Elmasry — AEL Digital Studio
-- ============================================================
-- The CS50 duck has been stolen!  The only clues are the files found
-- in the Fiftyville town hall.  This log reconstructs the investigation
-- step by step, narrowing the suspect pool until the thief, their
-- accomplice, and the escape city are identified.
-- ============================================================


-- ------------------------------------------------------------
-- STEP 1 — Read the crime scene report
-- The report for 2021-07-28 on Humphrey Street pins down when and
-- where the theft happened, so later searches can focus on the
-- witnesses and the bakery parking lot.
-- ------------------------------------------------------------
SELECT description
FROM crime_scene_reports
WHERE year = 2021
  AND month = 7
  AND day = 28
  AND street = 'Humphrey Street';

-- Findings: Theft of the CS50 duck took place at 10:15am at the
-- Humphrey Street bakery.  Three witnesses were interviewed that day,
-- and each transcript mentions the bakery.


-- ------------------------------------------------------------
-- STEP 2 — Read the witness interviews
-- The witnesses are Ruth, Eugene, and Raymond.  Their accounts list
-- the three breadcrumbs that drive the whole investigation.
-- ------------------------------------------------------------
SELECT name, transcript
FROM interviews
WHERE year = 2021
  AND month = 7
  AND day = 28;

-- Ruth:  the thief left the bakery parking lot within ten minutes.
-- Eugene: the thief withdrew money from the ATM on Leggett Street.
-- Raymond: the thief called someone for under a minute and asked that
--         person to buy a ticket on the earliest flight out of
--         Fiftyville tomorrow (2021-07-29).


-- ------------------------------------------------------------
-- STEP 3 — Cars that left the bakery parking lot
-- Ruth said the thief drove away within ten minutes of the 10:15am
-- theft.  The security logs for the bakery between 10:15 and 10:25
-- expose a short list of licence plates to cross-reference.
-- ------------------------------------------------------------
SELECT license_plate, activity, hour, minute
FROM bakery_security_logs
WHERE year = 2021
  AND month = 7
  AND day = 28
  AND hour = 10
  AND minute BETWEEN 15 AND 25
ORDER BY minute;

-- Finding: eight cars exited between 10:16 and 10:23.  Match these
-- plates against the people table to build the first suspect pool.


-- ------------------------------------------------------------
-- STEP 4 — ATM withdrawals on Leggett Street
-- Eugene saw the thief at the Leggett Street ATM.  Pull the account
-- numbers that had money withdrawn there on the morning of the theft.
-- ------------------------------------------------------------
SELECT account_number, amount
FROM atm_transactions
WHERE year = 2021
  AND month = 7
  AND day = 28
  AND atm_location = 'Leggett Street'
  AND transaction_type = 'withdraw';

-- Finding: eight withdrawals.  The bank_accounts table connects these
-- account numbers back to the people table.


-- ------------------------------------------------------------
-- STEP 5 — Phone calls under a minute
-- Raymond heard the thief call someone and talk for less than a
-- minute.  Every call on 2021-07-28 with a duration under 60 seconds
-- is a candidate.
-- ------------------------------------------------------------
SELECT caller, receiver, duration
FROM phone_calls
WHERE year = 2021
  AND month = 7
  AND day = 28
  AND duration < 60;


-- ------------------------------------------------------------
-- STEP 6 — The earliest flight out of Fiftyville
-- Raymond also heard the thief book a ticket on the earliest flight
-- out of Fiftyville on the next morning, 2021-07-29.  Sort that
-- morning's departures by time to find which one the thief boarded.
-- ------------------------------------------------------------
SELECT id, hour, minute, origin_airport_id, destination_airport_id
FROM flights
WHERE year = 2021
  AND month = 7
  AND day = 29
  AND origin_airport_id = (
      SELECT id
      FROM airports
      WHERE city = 'Fiftyville'
  )
ORDER BY hour, minute
LIMIT 1;

-- Finding: the 8:20am flight, id 36.  Identify its destination city.
SELECT city
FROM airports
WHERE id = (
    SELECT destination_airport_id
    FROM flights
    WHERE id = 36
);

-- Finding: the escape city is New York City (LaGuardia Airport).


-- ------------------------------------------------------------
-- STEP 7 — Cross-reference every clue
-- The thief must appear in all four candidate lists at once:
--   1. a car that left the bakery parking lot 10:15-10:25
--   2. a Leggett Street ATM withdrawal on 2021-07-28
--   3. a caller on a phone call under 60 seconds on 2021-07-28
--   4. a passenger aboard flight 36 on 2021-07-29
-- Intersecting the sets leaves exactly one person.
-- ------------------------------------------------------------
SELECT people.name
FROM people
JOIN bank_accounts ON bank_accounts.person_id = people.id
JOIN atm_transactions ON atm_transactions.account_number = bank_accounts.account_number
JOIN bakery_security_logs ON bakery_security_logs.license_plate = people.license_plate
JOIN phone_calls ON phone_calls.caller = people.phone_number
JOIN passengers ON passengers.passport_number = people.passport_number
WHERE atm_transactions.year = 2021
  AND atm_transactions.month = 7
  AND atm_transactions.day = 28
  AND atm_transactions.atm_location = 'Leggett Street'
  AND atm_transactions.transaction_type = 'withdraw'
  AND bakery_security_logs.year = 2021
  AND bakery_security_logs.month = 7
  AND bakery_security_logs.day = 28
  AND bakery_security_logs.hour = 10
  AND bakery_security_logs.minute BETWEEN 15 AND 25
  AND phone_calls.year = 2021
  AND phone_calls.month = 7
  AND phone_calls.day = 28
  AND phone_calls.duration < 60
  AND passengers.flight_id = 36;

-- Finding: the thief is Bruce.


-- ------------------------------------------------------------
-- STEP 8 — Identify the accomplice
-- Raymond said the thief called someone to buy the plane ticket.  The
-- person on the other end of Bruce's short call on 2021-07-28 is the
-- accomplice who booked the escape flight.
-- ------------------------------------------------------------
SELECT people.name
FROM people
JOIN phone_calls ON phone_calls.receiver = people.phone_number
WHERE phone_calls.year = 2021
  AND phone_calls.month = 7
  AND phone_calls.day = 28
  AND phone_calls.duration < 60
  AND phone_calls.caller = (
      SELECT phone_number
      FROM people
      WHERE name = 'Bruce'
  );

-- Finding: the accomplice is Robin.


-- ------------------------------------------------------------
-- CONCLUSION
-- The investigation is complete.
--
--   The THIEF is:      Bruce
--   The city the thief ESCAPED TO:  New York City
--   The ACCOMPLICE is: Robin
--
-- Bruce left the bakery parking lot at 10:18am, withdrew money from
-- the Leggett Street ATM, made a 45-second call to Robin asking for a
-- ticket, and boarded flight 36 from Fiftyville to New York City on
-- the morning of 2021-07-29.
-- ------------------------------------------------------------
