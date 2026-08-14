-- ==============================================================================
-- Project   : AEL Sovereign — CS50x 2026-2027
-- Module    : week7_movies_pset7
-- File      : movies/5.sql
-- Author    : Ayman Elmasry — AEL Digital Studio
-- ------------------------------------------------------------------------------
-- Description:
--   Query 5: List the titles and release years of all Harry Potter films, in
--   chronological order.
--
-- SQL logic:
--   A LIKE pattern match identifies franchise titles: the `%` wildcard means
--   "any characters may follow", so `LIKE "Harry Potter%"` captures every
--   title beginning with the literal substring "Harry Potter" (e.g. "Harry
--   Potter and the Sorcerer's Stone"). ORDER BY year then arranges the result
--   set chronologically from earliest release to latest.
--
--   Note: LIKE is case-insensitive for ASCII by default in SQLite, and the
--   % wildcard is anchored only at the pattern's end, which is exactly the
--   franchise-prefix semantics required.
--
-- Returns:
--   Two columns, `title` and `year`, for every Harry Potter film, sorted by
--   release year ascending.
-- ==============================================================================

SELECT title, year
FROM movies
WHERE title LIKE "Harry Potter%"
ORDER BY year;
