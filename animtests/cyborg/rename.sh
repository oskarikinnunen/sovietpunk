for f in *.obj; do mv "$f" "$(echo "$f" | sed s/cy__/cy_/)"; done
