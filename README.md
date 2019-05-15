# SubsetSumSolver
Skrypt build.sh służy tylko do budowania projektu.
Skryptowi tests.sh można podać następujące opcjonalne argumenty:
-b (żeby zbudować projekt przed uruchomieniem testów),
-p (żeby odpalić profil'era, na razie jest to gprof, może poszukam coś lepszego)
-g n (żeby wygenerować nową paczkę n testów do pliku in.txt za pomocą gen.py).
(! argumenty muszą być podane w tej kolejności)
Sam skrypt zawsze uruchamia naszą binarkę z in.txt jako inputem.
Cała najważniejsza logika jest w pliku Implementations/MainImpl.cpp.
