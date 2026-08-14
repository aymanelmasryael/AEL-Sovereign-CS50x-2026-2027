"""
CS50 Finance - Enterprise Helper Library
===============================================================================

This module supplies the four utilities on which the Finance application
and the CS50 check50 harness depend:

    apology(message, code)   -> render the styled apology page
    login_required(f)        -> decorator that guards authenticated routes
    lookup(symbol)           -> resolve a ticker symbol to name / price / symbol
    usd(value)               -> format a numeric value as United States Dollars

Quote adapter architecture
------------------------------------------------------------------------------
``lookup`` implements a *tiered* quote adapter so the application remains fully
operational regardless of whether a paid data key is available or the hosting
machine has no external network access at all:

    1. IEX Cloud              -> primary source, used when the ``API_KEY``
                                 environment variable is set (legacy
                                 distribution behaviour, preserved verbatim).
    2. Yahoo Finance (free)   -> zero-authentication JSON fallback that works
                                 with no API key, in the CS50 IDE and locally.
    3. Static demo dataset    -> offline seed data so the Buy / Sell /
                                 Portfolio lifecycle can be demonstrated on a
                                 fully closed network.

Each tier is an isolated function so a failure in any one of them silently
falls through to the next. The original CS50 contract is fully preserved:
``lookup`` returns a dict with keys ``name``, ``price``, ``symbol``, or None.
"""

import os
import urllib.parse
from functools import wraps

import requests
from flask import redirect, render_template, session


# ---------------------------------------------------------------------------
# apology - styled error page
# ---------------------------------------------------------------------------
def apology(message, code=400):
    """
    Render a polished apology page carrying an HTTP status code.

    The signature deliberately mirrors the CS50 distribution contract so the
    ``(top, bottom)`` variables expected by ``apology.html`` are untouched.
    Jinja auto-escapes the message, so no manual URL escaping is required.
    """
    return render_template("apology.html", top=code, bottom=message), code


# ---------------------------------------------------------------------------
# login_required - authentication guard
# ---------------------------------------------------------------------------
def login_required(f):
    """
    Decorate routes to require a logged-in user.

    Any request that reaches a decorated view without an authenticated
    ``session["user_id"]`` is transparently redirected to the login page.

    https://flask.palletsprojects.com/en/latest/patterns/viewdecorators/
    """
    @wraps(f)
    def decorated_function(*args, **kwargs):
        if session.get("user_id") is None:
            return redirect("/login")
        return f(*args, **kwargs)
    return decorated_function


# ---------------------------------------------------------------------------
# Quote adapters - tiered resolution
# ---------------------------------------------------------------------------
def _lookup_iex(symbol):
    """
    Fetch a quote from the IEX Cloud REST API using the ``API_KEY`` env var.

    Returns a normalized quote dict, or None if the key is missing, the
    network call fails, or the payload cannot be parsed.
    """
    api_key = os.environ.get("API_KEY")
    if not api_key:
        return None  # No key configured - defer to the next adapter tier.

    try:
        url = (
            "https://cloud.iexapis.com/stable/stock/"
            f"{urllib.parse.quote_plus(symbol)}/quote?token={api_key}"
        )
        response = requests.get(url, timeout=10)
        response.raise_for_status()
        quote = response.json()
        return {
            "name": quote["companyName"],
            "price": float(quote["latestPrice"]),
            "symbol": quote["symbol"],
        }
    except (requests.RequestException, KeyError, TypeError, ValueError):
        return None


def _lookup_yahoo(symbol):
    """
    Fetch a quote from Yahoo Finance's public chart endpoint.

    This free, key-free JSON API reports the regular-market price plus the
    company's long/short name for any traded symbol. Returns a normalized
    quote dict, or None when the symbol is unknown or the request fails.
    """
    try:
        url = (
            "https://query1.finance.yahoo.com/v8/finance/chart/"
            f"{urllib.parse.quote_plus(symbol)}?interval=1d&range=1d"
        )
        response = requests.get(
            url,
            timeout=10,
            headers={"User-Agent": "Mozilla/5.0 (CS50 Finance student app)"},
        )
        response.raise_for_status()

        results = response.json().get("chart", {}).get("result")
        if not results:
            return None  # Unknown ticker - Yahoo returns an empty result set.

        meta = results[0].get("meta", {})
        price = meta.get("regularMarketPrice")
        if price is None:
            return None

        name = meta.get("longName") or meta.get("shortName") or symbol
        return {
            "name": name,
            "price": float(price),
            "symbol": meta.get("symbol", symbol),
        }
    except (requests.RequestException, ValueError, KeyError, TypeError):
        return None


# Small, documented offline dataset so the whole buy/sell flow can be
# demonstrated on a closed network or when every remote source is down.
DEMO_QUOTES = {
    "AAPL": {"name": "Apple Inc.", "price": 178.72},
    "AMZN": {"name": "Amazon.com, Inc.", "price": 148.36},
    "GOOGL": {"name": "Alphabet Inc.", "price": 141.28},
    "META": {"name": "Meta Platforms, Inc.", "price": 486.01},
    "MSFT": {"name": "Microsoft Corporation", "price": 374.12},
    "NFLX": {"name": "Netflix, Inc.", "price": 482.55},
    "NVDA": {"name": "NVIDIA Corporation", "price": 601.99},
    "TSLA": {"name": "Tesla, Inc.", "price": 238.45},
}


def _lookup_demo(symbol):
    """
    Resolve a ticker from the bundled offline demo dataset.

    This is the last-resort adapter: it guarantees that common CS50 symbols
    (NFLX, AAPL, ...) return data even with no network and no API key.
    """
    record = DEMO_QUOTES.get(symbol.upper())
    if record is None:
        return None
    return {
        "name": record["name"],
        "price": record["price"],
        "symbol": symbol.upper(),
    }


def lookup(symbol):
    """
    Resolve a stock symbol to a normalized quote dict.

    Adapter priority: IEX Cloud -> Yahoo Finance (free) -> static demo data.
    Returns ``{"name", "price", "symbol"}`` on success, otherwise None.
    The symbol is normalized to uppercase before resolution.
    """
    if not symbol:
        return None

    normalized = symbol.strip().upper()
    if not normalized:
        return None

    for adapter in (_lookup_iex, _lookup_yahoo, _lookup_demo):
        result = adapter(normalized)
        if result is not None:
            return result

    # Every tier exhausted - the symbol does not resolve anywhere.
    return None


# ---------------------------------------------------------------------------
# usd - currency formatting
# ---------------------------------------------------------------------------
def usd(value):
    """Format a numeric value as United States Dollars, e.g. $12,345.67."""
    try:
        numeric = float(value)
    except (TypeError, ValueError):
        numeric = 0.0
    return f"${numeric:,.2f}"