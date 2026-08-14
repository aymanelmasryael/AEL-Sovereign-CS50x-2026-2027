-- ============================================================
-- AEL Sovereign — CS50x 2026-2027
-- Problem Set 7: Movies — Query 13
-- Author: Ayman Elmasry — AEL Digital Studio
-- ============================================================
-- Write a SQL query to list the names of all people who starred in a
-- movie in which Kevin Bacon (born in 1958) also starred.
--
-- The inner query finds every movie Kevin Bacon appeared in by joining
-- stars to people and filtering on his name AND birth year (Kevin Bacon
-- is part of the original "Bacon number" data, so the year disambiguates
-- him).  The outer query then selects everyone who starred in any of
-- those films, excluding Kevin Bacon himself.  DISTINCT removes
-- duplicates from the final list.
-- ============================================================

SELECT DISTINCT people.name
FROM people
JOIN stars ON stars.person_id = people.id
WHERE stars.movie_id IN (
        SELECT stars.movie_id
        FROM stars
        JOIN people ON people.id = stars.person_id
        WHERE people.name = 'Kevin Bacon'
          AND people.birth = 1958
    )
  AND people.name != 'Kevin Bacon';