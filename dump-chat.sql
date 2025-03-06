--
-- PostgreSQL database cluster dump
--

-- Started on 2025-02-19 05:29:56

SET default_transaction_read_only = off;

SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;

--
-- Roles
--

CREATE ROLE postgres;
ALTER ROLE postgres WITH SUPERUSER INHERIT CREATEROLE CREATEDB LOGIN REPLICATION BYPASSRLS;

--
-- User Configurations
--








--
-- Databases
--

--
-- Database "template1" dump
--

\connect template1

--
-- PostgreSQL database dump
--

-- Dumped from database version 17.2
-- Dumped by pg_dump version 17.2

-- Started on 2025-02-19 05:29:56

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET transaction_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

-- Completed on 2025-02-19 05:29:56

--
-- PostgreSQL database dump complete
--

--
-- Database "chat" dump
--

--
-- PostgreSQL database dump
--

-- Dumped from database version 17.2
-- Dumped by pg_dump version 17.2

-- Started on 2025-02-19 05:29:57

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET transaction_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

--
-- TOC entry 4907 (class 1262 OID 24597)
-- Name: chat; Type: DATABASE; Schema: -; Owner: postgres
--

CREATE DATABASE chat WITH TEMPLATE = template0 ENCODING = 'UTF8' LOCALE_PROVIDER = libc LOCALE = 'Russian_Russia.1251';


ALTER DATABASE chat OWNER TO postgres;

\connect chat

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET transaction_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- TOC entry 217 (class 1259 OID 24598)
-- Name: message; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.message (
    name character varying NOT NULL,
    msg character varying NOT NULL
);


ALTER TABLE public.message OWNER TO postgres;

--
-- TOC entry 218 (class 1259 OID 24603)
-- Name: priv_msg; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.priv_msg (
    from_user character varying NOT NULL,
    to_user character varying NOT NULL,
    msg character varying NOT NULL
);


ALTER TABLE public.priv_msg OWNER TO postgres;

--
-- TOC entry 219 (class 1259 OID 24608)
-- Name: session; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.session (
    id integer NOT NULL,
    login character varying NOT NULL
);


ALTER TABLE public.session OWNER TO postgres;

--
-- TOC entry 220 (class 1259 OID 24613)
-- Name: users; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.users (
    name character varying NOT NULL,
    pwd character varying NOT NULL
);


ALTER TABLE public.users OWNER TO postgres;

--
-- TOC entry 4898 (class 0 OID 24598)
-- Dependencies: 217
-- Data for Name: message; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.message (name, msg) FROM stdin;
Ђ«ҐЄб:	 19.2.2025 2.24-> ЏаЁўҐв ўбҐ¬\n
Stas:	 19.2.2025 2.25-> Hello world\n
\.


--
-- TOC entry 4899 (class 0 OID 24603)
-- Dependencies: 218
-- Data for Name: priv_msg; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.priv_msg (from_user, to_user, msg) FROM stdin;
Stas: 	Ђ«ҐЄб	 ЇаЁўҐв Ђ«ҐЄб, Є Є ¤Ґ« ?\n
\.


--
-- TOC entry 4900 (class 0 OID 24608)
-- Dependencies: 219
-- Data for Name: session; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.session (id, login) FROM stdin;
\.


--
-- TOC entry 4901 (class 0 OID 24613)
-- Dependencies: 220
-- Data for Name: users; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.users (name, pwd) FROM stdin;
Ђ«ҐЄб	123
Stas	123
\.


-- Completed on 2025-02-19 05:29:57

--
-- PostgreSQL database dump complete
--

-- Completed on 2025-02-19 05:29:57

--
-- PostgreSQL database cluster dump complete
--

