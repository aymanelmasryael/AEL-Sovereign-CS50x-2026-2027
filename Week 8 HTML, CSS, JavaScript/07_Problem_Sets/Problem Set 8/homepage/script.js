/* ============================================================================
   AEL DIGITAL STUDIO — CV / PORTFOLIO — Interaction Engine v1.0
   Author : Ayman Elmasry
   Purpose: Vanilla-JS micro-interactions for the CS50x week 8 "homepage":
            1. Mobile navigation toggle
            2. Sticky navbar state on scroll
            3. Scroll-reveal animations (IntersectionObserver)
            4. Animated skill meters on reveal
            5. Scroll-spy: highlight the active nav link
            6. Auto-updating footer year
   ========================================================================== */

document.addEventListener("DOMContentLoaded", () => {

    /* ------------------------------------------------------------------
       1. Mobile Navigation Toggle
    ------------------------------------------------------------------ */
    const navToggle = document.getElementById("nav-toggle");
    const navMenu = document.getElementById("nav-menu");

    if (navToggle && navMenu) {
        navToggle.addEventListener("click", () => {
            const expanded = navToggle.getAttribute("aria-expanded") === "true";
            navToggle.setAttribute("aria-expanded", String(!expanded));
            navMenu.classList.toggle("is-open");
        });
    }

    /* ------------------------------------------------------------------
       2. Sticky Navbar — shadow state on scroll
    ------------------------------------------------------------------ */
    const navbar = document.getElementById("navbar");

    const updateNavbarState = () => {
        if (!navbar) return;
        navbar.classList.toggle("is-scrolled", window.scrollY > 12);
    };

    updateNavbarState();
    window.addEventListener("scroll", updateNavbarState, { passive: true });

    /* ------------------------------------------------------------------
       3 & 4. Scroll-Reveal + Animated Skill Meters
       Uses IntersectionObserver so sections fade in the first time they
       enter the viewport. Skill meter fill-width is driven by the inline
       data-level attribute (a percentage 0-100).
    ------------------------------------------------------------------ */
    const revealables = document.querySelectorAll(".reveal, .meter-fill");

    if ("IntersectionObserver" in window) {
        const revealObserver = new IntersectionObserver(
            (entries) => {
                entries.forEach((entry) => {
                    if (!entry.isIntersecting) return;

                    const el = entry.target;

                    if (el.classList.contains("meter-fill")) {
                        // Drive the progress width from the data-level attribute.
                        const level = Math.min(100, Math.max(0, parseInt(el.dataset.level, 10) || 0));
                        el.style.setProperty("--level", level + "%");
                        el.classList.add("is-visible");
                    } else {
                        el.classList.add("is-visible");
                    }

                    // Only reveal each element once, then stop observing it.
                    revealObserver.unobserve(el);
                });
            },
            { threshold: 0.18 }
        );

        revealables.forEach((el) => revealObserver.observe(el));
    } else {
        // Graceful fallback: reveal everything immediately for older browsers.
        revealables.forEach((el) => {
            if (el.classList.contains("meter-fill")) {
                const level = Math.min(100, Math.max(0, parseInt(el.dataset.level, 10) || 0));
                el.style.setProperty("--level", level + "%");
            }
            el.classList.add("is-visible");
        });
    }

    /* ------------------------------------------------------------------
       5. Scroll-Spy — highlight the nav link of the visible section
    ------------------------------------------------------------------ */
    const navLinks = Array.from(document.querySelectorAll(".nav-link"));
    const sections = navLinks
        .map((link) => document.querySelector(link.getAttribute("href")))
        .filter((section) => section !== null);

    const setActiveLink = (id) => {
        navLinks.forEach((link) => {
            const isActive = link.getAttribute("href") === "#" + id;
            link.classList.toggle("is-active", isActive);
        });
    };

    const activeObserver = new IntersectionObserver(
        (entries) => {
            entries.forEach((entry) => {
                if (entry.isIntersecting) {
                    setActiveLink(entry.target.id);
                }
            });
        },
        { rootMargin: "-45% 0px -50% 0px", threshold: 0 }
    );

    sections.forEach((section) => activeObserver.observe(section));

    // Close the mobile menu after tapping any nav link (smooth-scroll target).
    navMenu?.querySelectorAll("a").forEach((link) => {
        link.addEventListener("click", () => {
            if (navMenu.classList.contains("is-open")) {
                navMenu.classList.remove("is-open");
                navToggle.setAttribute("aria-expanded", "false");
            }
        });
    });

    /* ------------------------------------------------------------------
       6. Auto-Updating Footer Year
    ------------------------------------------------------------------ */
    const yearEl = document.getElementById("year");
    if (yearEl) {
        yearEl.textContent = String(new Date().getFullYear());
    }
});
