-- ==============================================================================
-- Project   : AEL Sovereign — CS50x 2026-2027
-- Module    : week7_movies_pset7
-- File      : movies/2.sql
-- Author    : Ayman Elmasry — AEL Digital Studio
-- ------------------------------------------------------------------------------
-- Description:
--   Query 2: Find the birth year of the person named "Emma Stone".
--
-- SQL logic:
--   Simple filtered projection on the `people` table. The WHERE clause matches
--   on the exact string value "Emma Stone"; the SELECT clause emits only the
--   `birth` column. Because a person's name is effectively unique in this
--   dataset, a single row (hence a single birth year) is expected. No join is
--   needed: birth year is an intrinsic attribute of the `people` row.
--
-- Returns:
--   One column, `birth`, holding the birth year of Emma Stone.
-- ==============================================================================

SELECT birth
FROM people
WHERE name = "Emma Stone";
