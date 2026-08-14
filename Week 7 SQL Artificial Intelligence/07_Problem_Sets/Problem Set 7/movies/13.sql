-- ==============================================================================
-- Project   : AEL Sovereign — CS50x 2026-2027
-- Module    : week7_movies_pset7
-- File      : movies/13.sql
-- Author    : Ayman Elmasry — AEL Digital Studio
-- ------------------------------------------------------------------------------
-- Description:
--   Query 13: List the distinct names of all people who co-starred with Kevin
--   Bacon (the 1958-born actor), excluding Kevin Bacon himself.
--
-- SQL logic:
--   A "degrees of separation" join that walks people → their films → the cast
--   of those films, then isolates the target:
--     1. `people` p JOIN `stars` s ON s.person_id = p.id
--              — every candidate actor with their starring credits.
--     2. `stars` s JOIN `movies` m ON m.id = s.movie_id
--              — resolve each credit to the shared film.
--     3. `movies` m JOIN `stars` s2 ON s2.movie_id = m.id
--              — pull the ENTIRE cast of that same film (co-stars).
--     4. `stars` s2 JOIN `people` p2 ON p2.id = s2.person_id
--                  AND p2.name = "Kevin Bacon" AND p2.birth = 1958
--              — require one cast member to be the correct Kevin Bacon.
--     5. WHERE p.name != "Kevin Bacon"
--              — exclude Bacon himself (his own co-star row would otherwise
--                count him), keeping only his collaborators.
--     6. DISTINCT — deduplicate people who shared several Bacon films.
--
-- Returns:
--   Unique names of every actor who appeared in a film with Kevin Bacon.
-- ==============================================================================

SELECT DISTINCT p.name
FROM people AS p
JOIN stars AS s ON s.person_id = p.id
JOIN movies AS m ON m.id = s.movie_id
JOIN stars AS s2 ON s2.movie_id = m.id
JOIN people AS p2 ON p2.id = s2.person_id AND p2.name = "Kevin Bacon" AND p2.birth = 1958
WHERE p.name != "Kevin Bacon";