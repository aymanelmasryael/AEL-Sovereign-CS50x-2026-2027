-- ==============================================================================
-- Project   : AEL Sovereign — CS50x 2026-2027
-- Module    : week7_movies_pset7
-- File      : movies/12.sql
-- Author    : Ayman Elmasry — AEL Digital Studio
-- ------------------------------------------------------------------------------
-- Description:
--   Query 12: List the titles of all movies in which both Johnny Depp and
--   Helena Bonham Carter starred.
--
-- SQL logic:
--   A double self-fan-out join that requires BOTH actors on the same film.
--   The movie row is joined to the cast twice, once per actor:
--     1. `movies` JOIN `stars` s1 JOIN `people` p1
--              — first copy resolves the cast and filters to Depp
--                (p1.name = "Johnny Depp").
--     2. `movies` JOIN `stars` s2 JOIN `people` p2
--              — second copy, joined again on the SAME movie_id, filters to
--                Carter (p2.name = "Helena Bonham Carter").
--   Because each join arm shares the movie's id, a surviving row proves the
--   film appears in Depp's filmography AND in Carter's filmography — i.e.
--   both co-star in that movie. Alias tables s1/p1 and s2/p2 keep the two
--   cast walks independent and unambiguous.
--
-- Returns:
--   One `title` column listing films co-starring both actors.
-- ==============================================================================

SELECT title
FROM movies
JOIN stars AS s1 ON s1.movie_id = movies.id
JOIN people AS p1 ON p1.id = s1.person_id AND p1.name = "Johnny Depp"
JOIN stars AS s2 ON s2.movie_id = movies.id
JOIN people AS p2 ON p2.id = s2.person_id AND p2.name = "Helena Bonham Carter";