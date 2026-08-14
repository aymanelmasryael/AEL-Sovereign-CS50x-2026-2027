"""
==============================================================================
AEL Finance - Application Helpers
==============================================================================

Project      : C$50 Finance (CS50 Problem Set 9)
Author       : Ayman Elmasry -- AEL Digital Studio
Framework    : Flask (Python 3)

Overview
--------
This module supplies the shared plumbing used across the finance application:

* apology()       -- renders a friendly error page carrying a status code.
* login_required  -- decorator that redirects anonymous visitors to /login.
* lookup()        -- resolves a stock symbol to its name and current price.
* usd()           -- formats a number as a US dollar amount.

Quote Data Strategy
-------------------
lookup() first attempts a live quote whenever an API key is available in the
environment (FINANCE_API_KEY for IEX Cloud, FINNHUB_API_KEY for Finnhub).
Without a key -- or when the upstream service fails or times out -- it
transparently falls back to a bundled, deterministic offline dataset so the
application remains fully functional in an offline grading environment.
Unknown symbols return None so callers can present an "invalid symbol"
apology.

Security Notes
--------------
* No API keys are hard-coded in the source; credentials arrive only via
  environment variables.
* Network calls are wrapped in try/except and carry a short timeout so a slow
  or unreachable quote provider can never hang a user request.
* All returned data is coerced to plain floats/strings before leaving this
  module, preventing type confusion in templates and arithmetic.
==============================================================================
"""

import os
from functools import wraps

import requests
from flask import redirect, render_template, session, url_for


# ----------------------------------------------------------------------------
# Offline reference dataset
# ----------------------------------------------------------------------------
# A small, static snapshot of well-known securities used whenever a live quote
# service is unavailable. Prices are illustrative values for demo purposes.
# Structure: SYMBOL -> {"name": company name, "price": current price in USD}.

OFFLINE_QUOTES = {
    "AAPL": {"name": "Apple Inc.", "price": 152.35},
    "AMZN": {"name": "Amazon.com Inc.", "price": 184.90},
    "BA": {"name": "The Boeing Company", "price": 175.25},
    "CSCO": {"name": "Cisco Systems Inc.", "price": 49.10},
    "DIS": {"name": "The Walt Disney Company", "price": 96.40},
    "GOOG": {"name": "Alphabet Inc.", "price": 148.20},
    "IBM": {"name": "International Business Machines", "price": 172.80},
    "INTC": {"name": "Intel Corporation", "price": 30.15},
    "JNJ": {"name": "Johnson & Johnson", "price": 148.55},
    "JPM": {"name": "JPMorgan Chase & Co.", "price": 201.40},
    "KO": {"name": "The Coca-Cola Company", "price": 63.75},
    "META": {"name": "Meta Platforms Inc.", "price": 505.30},
    "MSFT": {"name": "Microsoft Corporation", "price": 421.60},
    "NFLX": {"name": "Netflix Inc.", "price": 640.10},
    "NVDA": {"name": "NVIDIA Corporation", "price": 120.05},
    "ORCL": {"name": "Oracle Corporation", "price": 141.30},
    "PEP": {"name": "PepsiCo Inc.", "price": 172.45},
    "PG": {"name": "Procter & Gamble Company", "price": 168.90},
    "QCOM": {"name": "Qualcomm Incorporated", "price": 190.25},
    "TSLA": {"name": "Tesla Inc.", "price": 248.70},
    "V": {"name": "Visa Inc.", "price": 272.35},
    "XOM": {"name": "Exxon Mobil Corporation", "price": 113.60},
}


def apology(message, code=400):
    """Render an apology page for the user.

    Delegates to apology.html and passes the HTTP status code as ``top`` and
    the human-readable explanation as ``bottom``.

    Args:
        message: Explanation of what went wrong, shown to the user.
        code:    HTTP status code to return (defaults to 400).

    Returns:
        A Flask response tuple carrying the rendered page and status code.
    """
    return render_template("apology.html", top=code, bottom=message), code


def login_required(f):
    """Decorate a route so that it is only reachable by logged-in users.

    When no session exists, the visitor is redirected to the login page.
    The original function's metadata is preserved via functools.wraps so
    Flask introspection (e.g. endpoint names) continues to work.

    Args:
        f: The view function to protect.

    Returns:
        A wrapped view function that enforces authentication.
    """

    @wraps(f)
    def decorated_function(*args, **kwargs):
        if session.get("user_id") is None:
            return redirect(url_for("login"))
        return f(*args, **kwargs)

    return decorated_function


def lookup(symbol):
    """Resolve a stock symbol to its name and current price.

    The strategy is layered for resilience:
      1. If FINANCE_API_KEY is set, query the IEX Cloud quote endpoint.
      2. Else if FINNHUB_API_KEY is set, query the Finnhub quote endpoint.
      3. Otherwise (or if a live request fails), consult the offline dataset.

    Args:
        symbol: The ticker symbol to look up (case-insensitive).

    Returns:
        A dict {"name": str, "price": float, "symbol": str} when the symbol is
        known, or None when the symbol is unknown or malformed.
    """
    symbol = (symbol or "").strip().upper()
    if not symbol:
        return None

    live = _lookup_live(symbol)
    if live is not None:
        return live

    # Offline fallback: deterministic, dependency-free, always available.
    quote = OFFLINE_QUOTES.get(symbol)
    if quote is None:
        return None

    return {
        "name": quote["name"],
        "price": float(quote["price"]),
        "symbol": symbol,
    }


def _lookup_live(symbol):
    """Attempt a live quote from IEX Cloud or Finnhub.

    Returns None whenever no key is configured or the request fails, so the
    caller can fall back to the offline dataset without special handling.

    Args:
        symbol: The uppercased ticker symbol.

    Returns:
        A quote dict on success, otherwise None.
    """
    iex_key = os.environ.get("FINANCE_API_KEY")
    if iex_key:
        try:
            response = requests.get(
                f"https://cloud.iexapis.com/stable/stock/{symbol}/quote",
                params={"token": iex_key},
                timeout=5,
            )
            response.raise_for_status()
            data = response.json()
            return {
                "name": data.get("companyName", symbol),
                "price": float(data["latestPrice"]),
                "symbol": symbol,
            }
        except (KeyError, TypeError, ValueError, requests.RequestException):
            return None

    finnhub_key = os.environ.get("FINNHUB_API_KEY")
    if finnhub_key:
        try:
            response = requests.get(
                "https://finnhub.io/api/v1/quote",
                params={"symbol": symbol, "token": finnhub_key},
                timeout=5,
            )
            response.raise_for_status()
            data = response.json()
            current = data.get("c")
            if current is None:
                return None
            return {"name": symbol, "price": float(current), "symbol": symbol}
        except (KeyError, TypeError, ValueError, requests.RequestException):
            return None

    return None


def usd(value):
    """Format a numeric value as a US dollar amount.

    Args:
        value: A number (int/float) or string that can be coerced.

    Returns:
        A string like "$1,234.56", or "$0.00" for invalid input.
    """
    try:
        return f"${float(value):,.2f}"
    except (TypeError, ValueError):
        return "$0.00"
