# Kalkulator Waris Islami

Sebuah website yang membantu masyarakat untuk menghitung harta waris sesuai akidah Islam berdasarkan hukum Faraidh.

## Fitur

- **Kalkulator Waris** - Hitung pembagian harta waris secara akurat
- **Tabel Distribusi** - Lihat hasil pembagian dalam tabel rinci
- **Panduan Lengkap** - Pelajari hukum waris Islam dari dasar

## Struktur Website

```
Kalkulator-Waris/
├── index.html          # Homepage
├── calculator.html     # Halaman kalkulator waris
├── guide.html          # Panduan hukum waris Islam
├── app.js              # Logic JavaScript untuk kalkulator
├── waris_module.js     # WebAssembly module (Emscripten)
├── waris_module.wasm   # Binary WebAssembly
├── tugasakhir.cpp      # Source code C++ perhitungan waris
└── README.md
```

## Cara Menggunakan

1. Buka `index.html` di browser
2. Klik "Mulai Menghitung" untuk ke kalkulator
3. Isi data pewaris dan ahli waris
4. Klik "Hitung Pembagian Waris"
5. Lihat hasil perhitungan

## Teknologi

- HTML, Tailwind CSS, JavaScript
- C++ dengan Emscripten (WebAssembly)

## Lisensi

Projek ini dibuat untuk tugas akir.
