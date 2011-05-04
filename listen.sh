#!/bin/sh
lilypond --pdf out.ly && timidity out.midi
