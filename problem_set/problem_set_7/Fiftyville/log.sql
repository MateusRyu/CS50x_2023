-- Keep a log of any SQL queries you execute as you solve the mystery.
-- All you know is that the theft took place on July 28, 2021 and that it took place on Humphrey Street.

-- Start by looking for a crime scene report that matches the date and the location of the crime.
SELECT id, description
    FROM crime_scene_reports
    WHERE street = 'Humphrey Street'
        AND year = 2021
        AND month = 7
        AND day = 28;

-- There two report thats match either date and location, but only rreport 295 match with a theft of the cs50 duck.
-- Description of report 295:
-- "Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery. Interviews were conducted today
-- with three witnesses who were present at the time – each of their interview transcripts mentions the bakery."

-- Look for the interviews with the witnesses
SELECT id, name, transcript
    FROM interviews
    WHERE year = 2021
        AND month = 7
        AND day = 28
        AND transcript LIKE '%bakery%';

-- Look for the bakery security logs that around 10 minutes of the theft, as said by Ruth, but I will add 5 min for security.
SELECT id, minute, license_plate
    FROM bakery_security_logs
    WHERE year = 2021
        AND month = 7
        AND day = 28
        AND hour = 10
        AND minute > 0
        AND minute < 30
        AND activity = 'exit'
    ORDER BY minute;

-- Look for withdraw at ATM on Leggett Street at the day of crime, as said by Eugene
SELECT id, account_number, amount
    FROM atm_transactions
        WHERE atm_location = 'Leggett Street'
            AND transaction_type = 'withdraw'
            AND year = 2021
            AND month = 7
            AND day = 28;

-- Look for phone calls to discover more info about  the thief and his cumplice, as said by Raymond
SELECT id, caller, receiver, duration
    FROM phone_calls
    WHERE duration < 60
        AND year = 2021
        AND month = 7
        AND day = 28;

-- look for the passengers of the earliest flight out of Fiftyville one day after the crime and the flights id
SELECT flights.id, passengers.passport_number, flights.origin_airport_id as 'origin', flights.destination_airport_id as 'destination', flights.hour, flights.minute
    FROM passengers
    JOIN flights
        ON flights.id = passengers.flight_id
    JOIN airports
        ON airports.id = flights.origin_airport_id
    WHERE airports.city = 'Fiftyville'
        AND flights.id IN (
            SELECT flights.id
                FROM flights
                WHERE flights.year = 2021
                    AND flights.month = 7
                    AND flights.day = 29
                ORDER BY year, month, day, hour, minute
                LIMIT 1
        );

-- Look to the airports
SELECT * FROM airports WHERE id = 8 OR id = 4;

-- Look for the passagers data
SELECT people.id, people.name, people.phone_number, people.license_plate, bank_accounts.account_number
    FROM people
    LEFT JOIN bank_accounts
        ON bank_accounts.person_id = people.id
    WHERE people.passport_number IN (
        SELECT passengers.passport_number
        FROM passengers
        JOIN flights
            ON flights.id = passengers.flight_id
        JOIN airports
            ON airports.id = flights.origin_airport_id
        WHERE flights.id = 36
        );

-- Get the passenger plate logged by bakery security around the time of crime
SELECT id, hour, minute, license_plate
    FROM bakery_security_logs
    WHERE year = 2021
        AND month = 7
        AND day = 28
        AND hour = 10
        AND minute > 0
        AND minute < 30
        AND activity = 'exit'
        and license_plate IN (
            SELECT people.license_plate
                FROM people
                LEFT JOIN bank_accounts
                    ON bank_accounts.person_id = people.id
                WHERE people.passport_number IN (
                    SELECT passengers.passport_number
                    FROM passengers
                    JOIN flights
                        ON flights.id = passengers.flight_id
                    JOIN airports
                        ON airports.id = flights.origin_airport_id
                    WHERE flights.id = 36
                    )
        )
    ORDER BY minute;

-- Get suspects data, based  on the bakery security (last query)
SELECT people.id, people.name, people.phone_number, people.license_plate, bank_accounts.account_number
    FROM people
    LEFT JOIN bank_accounts
        ON bank_accounts.person_id = people.id
    WHERE people.license_plate = '94KL13X'
        OR people.license_plate = '4328GD8'
        OR people.license_plate = 'G412CB7'
        OR people.license_plate = '0NTHK55';

-- Get passager who withdraw at the ATM as described by Eugene
SELECT id, account_number, amount
    FROM atm_transactions
        WHERE atm_location = 'Leggett Street'
            AND transaction_type = 'withdraw'
            AND year = 2021
            AND month = 7
            AND day = 28
            AND account_number IN (
                SELECT bank_accounts.account_number
                    FROM bank_accounts
                    JOIN people
                        ON bank_accounts.person_id = people.id
                    WHERE people.passport_number IN (
                        SELECT passengers.passport_number
                        FROM passengers
                        JOIN flights
                            ON flights.id = passengers.flight_id
                        JOIN airports
                            ON airports.id = flights.origin_airport_id
                        WHERE flights.id = 36
                        )
            );

-- Get suspects data, based on the withdraw
SELECT people.id, people.name, people.phone_number, people.license_plate, bank_accounts.account_number
    FROM people
    LEFT JOIN bank_accounts
        ON bank_accounts.person_id = people.id
    WHERE bank_accounts.account_number = '28500762'
        OR bank_accounts.account_number = '28296815'
        OR bank_accounts.account_number = '76054385'
        OR bank_accounts.account_number = '49610011';

-- Look for numbers of suspicious callers who were recorded by the bakery security and who withdraw money from the ATM, based on the testimony said by Raimond
SELECT id, caller, receiver
    FROM phone_calls
    WHERE duration < 60
        AND year = 2021
        AND month = 7
        AND day = 28
        AND (caller IN (
            SELECT phone_number
                FROM people
                WHERE id = '467400'
                    OR id = '686048'
        )
        OR receiver IN (
            SELECT phone_number
                FROM people
                WHERE id = '467400'
                    OR id = '686048'
        ));

-- Bruce is probably the thief because he appears in all the evidences brough by the three witnesses
-- Let's me see who is his accomplice
SELECT id, name
    FROM people
    WHERE phone_number = '(375) 555-8161'
