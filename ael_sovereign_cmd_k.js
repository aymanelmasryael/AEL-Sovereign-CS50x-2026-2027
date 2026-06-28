/**
 * AEL Sovereign AI Command Palette (Cmd+K / Ctrl+K)
 * Designed by Ayman Elmasry - AI Prompt Engineer & Computational Creative Director
 * Universal standalone module that injects a high-performance floating command hub.
 */

class AELCommandPalette {
    constructor() {
        this.init();
    }

    init() {
        if (document.getElementById('ael-cmd-palette')) return;

        // Inject Stylesheet
        const style = document.createElement('style');
        style.innerHTML = `
            #ael-palette-overlay {
                position: fixed;
                inset: 0;
                background: rgba(1, 6, 17, 0.85);
                backdrop-filter: blur(16px);
                -webkit-backdrop-filter: blur(16px);
                z-index: 99998;
                display: none;
                align-items: flex-start;
                justify-content: center;
                padding-top: 12vh;
                font-family: 'Inter', -apple-system, sans-serif;
            }
            #ael-palette-modal {
                width: 100%;
                max-width: 680px;
                background: #030b1c;
                border: 1px solid rgba(0, 116, 255, 0.55);
                border-radius: 24px;
                box-shadow: 0 20px 60px rgba(0,0,0,0.5), 0 0 40px rgba(0,116,255,0.2);
                overflow: hidden;
                animation: aelPaletteScale 0.2s ease-out;
            }
            @keyframes aelPaletteScale {
                from { opacity: 0; transform: scale(0.96); }
                to { opacity: 1; transform: scale(1); }
            }
            #ael-palette-search-container {
                display: flex;
                align-items: center;
                padding: 22px 28px;
                border-bottom: 1px solid rgba(0, 116, 255, 0.3);
                gap: 18px;
            }
            #ael-palette-search-container i {
                font-size: 22px;
                color: #0074FF;
            }
            #ael-palette-input {
                width: 100%;
                background: transparent;
                border: none;
                color: #ffffff;
                font-size: 20px;
                outline: none;
                font-weight: 500;
            }
            #ael-palette-input::placeholder { color: #5a6882; }
            #ael-palette-results {
                max-height: 420px;
                overflow-y: auto;
                padding: 16px;
            }
            .ael-palette-item {
                display: flex;
                align-items: center;
                justify-content: space-between;
                padding: 16px 22px;
                border-radius: 16px;
                color: #a0aab2;
                cursor: pointer;
                transition: all 0.2s ease;
                margin-bottom: 6px;
                text-decoration: none;
            }
            .ael-palette-item:hover, .ael-palette-item.selected {
                background: #0074FF;
                color: #ffffff;
            }
            .ael-palette-item-left { display: flex; align-items: center; gap: 16px; font-size: 16px; font-weight: 600; }
            .ael-palette-item-left i { font-size: 20px; }
            .ael-palette-item-shortcut {
                background: rgba(255,255,255,0.1);
                padding: 4px 10px;
                border-radius: 8px;
                font-size: 12px;
                font-family: monospace;
            }
            #ael-palette-footer {
                padding: 14px 24px;
                background: #010611;
                border-top: 1px solid rgba(0, 116, 255, 0.2);
                color: #5a6882;
                font-size: 13px;
                display: flex;
                justify-content: space-between;
            }
        `;
        document.head.appendChild(style);

        // Inject DOM Overlay
        const overlay = document.createElement('div');
        overlay.id = 'ael-palette-overlay';
        overlay.innerHTML = `
            <div id="ael-palette-modal" onclick="event.stopPropagation()">
                <div id="ael-palette-search-container">
                    <i class="fas fa-magnifying-glass"></i>
                    <input id="ael-palette-input" type="text" placeholder="Search CS50x Citadel... (Try 'Exams', 'IDE', 'Prompt')" autocomplete="off">
                </div>
                <div id="ael-palette-results"></div>
                <div id="ael-palette-footer">
                    <span>⚡ AEL Sovereign Command Palette</span>
                    <span>Navigate: <b>↑ ↓</b> &middot; Select: <b>Enter</b> &middot; Close: <b>Esc</b></span>
                </div>
            </div>
        `;
        document.body.appendChild(overlay);

        this.overlay = overlay;
        this.input = document.getElementById('ael-palette-input');
        this.resultsContainer = document.getElementById('ael-palette-results');

        this.items = [
            { title: "Master Dashboard (Home)", url: "docs.html", icon: "fa-home", category: "Core" },
            { title: "CS50x Exams & Solutions Citadel", url: "cs50x_exams_and_solutions.html", icon: "fa-graduation-cap", category: "Exams" },
            { title: "Live WebAssembly IDE & Pyodide Sandbox", url: "cs50x_live_ide.html", icon: "fa-terminal", category: "IDE" },
            { title: "AI Prompt Engineering & Jailbreak Defenses", url: "cs50x_ai_prompt_engineering.html", icon: "fa-brain", category: "AI" },
            { title: "Week 0: Scratch Solutions", url: "cs50x_exams_and_solutions.html#tab-w0", icon: "fa-shapes", category: "Exams" },
            { title: "Week 1: C Programming & Stack Frames", url: "cs50x_exams_and_solutions.html#tab-w1", icon: "fa-code", category: "Exams" },
            { title: "Week 2: Arrays & Vigenere Cipher", url: "cs50x_exams_and_solutions.html#tab-w2", icon: "fa-list-ol", category: "Exams" },
            { title: "Week 3: Algorithms & Electoral Ties", url: "cs50x_exams_and_solutions.html#tab-w3", icon: "fa-route", category: "Exams" },
            { title: "Week 4: Memory Forensics & Bitmaps", url: "cs50x_exams_and_solutions.html#tab-w4", icon: "fa-memory", category: "Exams" },
            { title: "Week 5: Data Structures & Tries", url: "cs50x_exams_and_solutions.html#tab-w5", icon: "fa-network-wired", category: "Exams" },
            { title: "Week 6: Python & DNA Sequencing", url: "cs50x_exams_and_solutions.html#tab-w6", icon: "fa-dna", category: "Exams" },
            { title: "Week 7: SQL Databases & AI Investigation", url: "cs50x_exams_and_solutions.html#tab-w7", icon: "fa-database", category: "Exams" },
            { title: "Week 8: Web Architecture & DOM Lifecycle", url: "cs50x_exams_and_solutions.html#tab-w8", icon: "fa-globe", category: "Exams" },
            { title: "Week 9: Flask & XSS Fortification", url: "cs50x_exams_and_solutions.html#tab-w9", icon: "fa-server", category: "Exams" },
            { title: "Week 10: The End & Timing Attack Defense", url: "cs50x_exams_and_solutions.html#tab-w10", icon: "fa-crown", category: "Exams" },
            { title: "Master Interactive Simulator (Weeks 0–10)", url: "cs50x_exams_and_solutions.html#tab-sim", icon: "fa-gamepad", category: "Simulator" }
        ];

        this.selectedIndex = 0;
        this.attachEvents();
    }

    attachEvents() {
        // Toggle on Cmd+K / Ctrl+K
        window.addEventListener('keydown', (e) => {
            if ((e.metaKey || e.ctrlKey) && e.key === 'k') {
                e.preventDefault();
                this.toggle();
            }
            if (e.key === 'Escape' && this.overlay.style.display === 'flex') {
                this.close();
            }
            if (this.overlay.style.display === 'flex') {
                if (e.key === 'ArrowDown') {
                    e.preventDefault();
                    this.selectedIndex = (this.selectedIndex + 1) % this.displayedItems.length;
                    this.renderResults();
                }
                if (e.key === 'ArrowUp') {
                    e.preventDefault();
                    this.selectedIndex = (this.selectedIndex - 1 + this.displayedItems.length) % this.displayedItems.length;
                    this.renderResults();
                }
                if (e.key === 'Enter') {
                    e.preventDefault();
                    if (this.displayedItems[this.selectedIndex]) {
                        window.location.href = this.displayedItems[this.selectedIndex].url;
                    }
                }
            }
        });

        this.overlay.addEventListener('click', () => this.close());
        this.input.addEventListener('input', () => {
            this.selectedIndex = 0;
            this.filterResults(this.input.value);
        });
    }

    toggle() {
        if (this.overlay.style.display === 'flex') {
            this.close();
        } else {
            this.overlay.style.display = 'flex';
            this.input.value = '';
            this.filterResults('');
            this.input.focus();
        }
    }

    close() {
        this.overlay.style.display = 'none';
    }

    filterResults(query) {
        query = query.toLowerCase().trim();
        if (!query) {
            this.displayedItems = this.items;
        } else {
            this.displayedItems = this.items.filter(item => 
                item.title.toLowerCase().includes(query) || item.category.toLowerCase().includes(query)
            );
        }
        this.renderResults();
    }

    renderResults() {
        this.resultsContainer.innerHTML = '';
        if (this.displayedItems.length === 0) {
            this.resultsContainer.innerHTML = `<div style="color:#5a6882; padding:20px; text-align:center;">No sovereign records found matching your query.</div>`;
            return;
        }

        this.displayedItems.forEach((item, idx) => {
            const el = document.createElement('a');
            el.href = item.url;
            el.className = `ael-palette-item ${idx === this.selectedIndex ? 'selected' : ''}`;
            el.innerHTML = `
                <div class="ael-palette-item-left">
                    <i class="fas ${item.icon}"></i>
                    <span>${item.title}</span>
                </div>
                <div class="ael-palette-item-shortcut">${item.category}</div>
            `;
            el.addEventListener('mouseenter', () => {
                this.selectedIndex = idx;
                this.renderResults();
            });
            this.resultsContainer.appendChild(el);
        });
    }
}

document.addEventListener('DOMContentLoaded', () => {
    window.aelCommandPalette = new AELCommandPalette();
});
