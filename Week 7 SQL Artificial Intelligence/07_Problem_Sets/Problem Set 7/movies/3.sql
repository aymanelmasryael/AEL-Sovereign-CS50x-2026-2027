-- ==============================================================================
-- Project   : AEL Sovereign — CS50x 2026-2027
-- Module    : week7_movies_pset7
-- File      : movies/3.sql
-- Author    : Ayman Elmasry — AEL Digital Studio
-- ------------------------------------------------------------------------------
-- Description:
--   Query 3: List the titles of all movies released in 2018 or later, in
--   alphabetical order.
--
-- SQL logic:
--   A range predicate `year >= 2018` filters the `movies` table, and an
--   ORDER BY clause sorts the surviving titles alphabetically (ASC is the
--   default collation order for TEXT). Two logical layers, in execution order:
--     1. WHERE  — row-level filter (keeps only 2018+ releases).
--     2. ORDER  — result-set sort (lexicographic ascending by title).
--   No join is necessary; all data is local to `movies`.
--
-- Returns:
--   One column, `title`, of every 2018-or-later movie, sorted A→Z.
-- ==============================================================================

SELECT title
FROM movies
WHERE year >= 2018
ORDER BY title;
