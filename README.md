# self_checksum

ENG:

Repository contains main.c, which self checking checksum, and utility calc_checksum.c. Both of this sources use libcrc32, initially forked from: `https://github.com/ETrun/crc32`; It was modified and compiled in static lib.

Launch order:
```
make
./chksum
./source
```

What the point of this?

Compiled `main.c` - `source`, contain special section `.crc32_checksum`. Compiled  and launched `calc_checksum.c` - `chksum`, will caclulate crc32 for `source` (actually, it will calculate crc32 on buffer, which will contain `source` bin, except of data from section `.crc32_checksum`) and put it in ELF of `source` in section `.crc32_checksum`.

Then, launched `source` will also calc crc32 on himself (same thing, calculate crc32 on buffer, which will contain `source`, except of data from section `.crc32_checksum`) and compare calculated crc32 hash with crc32 from `crc32_checksum` section. If this operation is sucess, that means file integrity of `source` was not violated and self checksum is successful.

RU:

Репозиторий содержит `main.c`, проверяющий контрольную сумму самого себя, и утилиту `calc_checksum.c`. Оба этих исходных файла используют `libcrc32`, изначально полученную из: `https://github.com/ETrun/crc32`; Исходный код был изменен и скомпилирован в статическую библиотеку.

Порядок запуска:
```
make
./chksum
./source
```

Какой в этом смысл?

Скомпилированный `main.c` -`source`, содержит специальную секцию `.crc32_checksum`. Скомпилированный и запущенный `calc_checksum.c` -` chksum`, будет рассчитывать crc32 контрольную сумму для `source` (фактически, рассчёт crc32 будет произведён на буфер, который будет содержать `source`, но кроме данных из секции `.crc32_checksum`) и поместит его в ELF `source` в секцию `.crc32_checksum`.

Затем, запущенный `source` также вычислит crc32 контрольную сумму на себя (то же самое, рассчитает crc32 на буфер, который будет содержать `source`, но за исключением данных из секции `.crc32_checksum`) и сравнит вычисленный хеш crc32 с crc32 из секции` crc32_checksum`. Если эта операция является успешной, это означает, что целостность файла `source` не была нарушена.
