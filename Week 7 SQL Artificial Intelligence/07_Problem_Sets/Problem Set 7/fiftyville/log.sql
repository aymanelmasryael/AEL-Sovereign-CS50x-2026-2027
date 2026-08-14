-- Keep a log of any SQL queries you execute as you solve the mystery.
SELECT * FROM crime_scene_reports WHERE day = 28 AND street = "Humphrey Street"; -- crime
SELECT transcript FROM interviews WHERE day = 28 AND month = 7;
SELECT * FROM phone_calls WHERE month = 7 AND day = 28 AND duration < 60;, -- "called someone and talked less ten a minute"
SELECT full_name, city FROM airports WHERE id = 4;
-- "Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away."
-- "the thief there withdrawing some money on Leggett Street."
-- "they were planning to take the earliest flight out of Fiftyville tomorrow."
