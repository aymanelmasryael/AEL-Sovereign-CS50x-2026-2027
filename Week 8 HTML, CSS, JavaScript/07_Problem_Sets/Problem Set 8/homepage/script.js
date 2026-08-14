/* ============================================================
   AEL Digital Studio — Homepage Interactions
   Mobile nav toggle, sticky nav, scroll reveal, smooth scroll,
   automatic footer year. All vanilla JavaScript, no dependencies.
   ============================================================ */

(function () {
    "use strict";

    /* --------------------------------------------------------
       1. MOBILE NAVIGATION TOGGLE
    -------------------------------------------------------- */
    const navToggle = document.getElementById("nav-toggle");
    const navList = document.getElementById("nav-list");

    if (navToggle && navList) {
        navToggle.addEventListener("click", function () {
            const isOpen = navList.classList.toggle("is-open");
            navToggle.classList.toggle("is-open", isOpen);
            navToggle.setAttribute("aria-expanded", String(isOpen));
            navToggle.setAttribute("aria-label", isOpen ? "Close navigation menu" : "Open navigation menu");
        });

        // Close the menu automatically when a link is chosen
        navList.addEventListener("click", function (event) {
            if (event.target.closest("a")) {
                navList.classList.remove("is-open");
                navToggle.classList.remove("is-open");
                navToggle.setAttribute("aria-expanded", "false");
            }
        });
    }

    /* --------------------------------------------------------
       2. STICKY NAVIGATION (frosted glass on scroll)
    -------------------------------------------------------- */
    const header = document.getElementById("site-header");

    if (header) {
        // Immediate check so the header looks right on refresh
        updateHeaderState();

        // Debounced scroll listener to avoid excessive style recalculations
        let scrollFrame = false;
        window.addEventListener(
            "scroll",
            function () {
                if (!scrollFrame) {
                    window.requestAnimationFrame(updateHeaderState);
                    scrollFrame = true;
                }
            },
            { passive: true }
        );

        function updateHeaderState() {
            header.classList.toggle("is-scrolled", window.scrollY > 12);
            scrollFrame = false;
        }
    }

    /* --------------------------------------------------------
       3. ACTIVE NAV LINK (optional scroll-spy)
    -------------------------------------------------------- */
    const sections = document.querySelectorAll("main section[id]");
    const navLinks = document.querySelectorAll('.nav-list a[href^="#"]');

    if ("IntersectionObserver" in window && sections.length && navLinks.length) {
        const spy = new IntersectionObserver(
            function (entries) {
                entries.forEach(function (entry) {
                    if (entry.isIntersecting) {
                        navLinks.forEach(function (link) {
                            const matches = link.getAttribute("href") === "#" + entry.target.id;
                            link.classList.toggle("is-active", matches);
                        });
                    }
                });
            },
            { rootMargin: "-45% 0px -50% 0px", threshold: 0 }
        );

        sections.forEach(function (section) {
            spy.observe(section);
        });
    }

    /* --------------------------------------------------------
       4. SCROLL REVEAL (IntersectionObserver with fallback)
    -------------------------------------------------------- */
    const revealItems = document.querySelectorAll(".reveal");

    function showImmediately() {
        revealItems.forEach(function (item) {
            item.classList.add("is-visible");
        });
    }

    if ("IntersectionObserver" in window) {
        const revealObserver = new IntersectionObserver(
            function (entries) {
                entries.forEach(function (entry) {
                    if (entry.isIntersecting) {
                        entry.target.classList.add("is-visible");
                        revealObserver.unobserve(entry.target); // animate once
                    }
                });
            },
            { rootMargin: "0px 0px -10% 0px", threshold: 0.08 }
        );

        revealItems.forEach(function (item) {
            revealObserver.observe(item);
        });
    } else {
        // Older browsers: never hide content, just show it
        showImmediately();
    }

    /* --------------------------------------------------------
       5. SMOOTH SCROLLING (progressive enhancement)
    -------------------------------------------------------- */
    // Respect users who prefer reduced motion
    const prefersReducedMotion = window.matchMedia("(prefers-reduced-motion: reduce)").matches;

    document.querySelectorAll('a[href^="#"]').forEach(function (link) {
        link.addEventListener("click", function (event) {
            const targetId = link.getAttribute("href");
            if (targetId.length < 2) return; // bare "#"

            const target = document.querySelector(targetId);
            if (!target) return;

            event.preventDefault();

            // Native smooth scrolling when supported and motion is allowed
            if ("scrollBehavior" in document.documentElement.style && !prefersReducedMotion) {
                target.scrollIntoView({ behavior: "smooth", block: "start" });
            } else {
                // Hard fallback: honor scroll-padding-top via CSS by scrolling to offset
                const headerHeight = header ? header.offsetHeight : 0;
                const top = target.getBoundingClientRect().top + window.pageYOffset - headerHeight;
                window.scrollTo({ top: Math.max(top, 0), behavior: "auto" });
            }

            // Keep focus where the user navigated (accessibility)
            target.setAttribute("tabindex", "-1");
            target.focus({ preventScroll: true });
        });
    });

    /* --------------------------------------------------------
       6. AUTOMATIC FOOTER YEAR
    -------------------------------------------------------- */
    const yearEl = document.getElementById("footer-year");
    if (yearEl) {
        yearEl.textContent = String(new Date().getFullYear());
    }
})();
