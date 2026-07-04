
let hitungWarisCplusPlus;

Module.onRuntimeInitialized = () => {
    // Menghubungkan fungsi C++ ke JavaScript menggunakan cwrap
    hitungWarisCplusPlus = Module.cwrap('hitungWarisWasm', null, [
        'number', 'number', 'number', 'number', 'number', 'number', 'number', 'number', 'number'
    ]);
    console.log("WebAssembly C++ Berhasil Dimuat!");
};

const jenisKelaminSelect = document.getElementById('jenisKelaminMayit');
const labelPasangan = document.getElementById('labelPasangan');
const formWaris = document.getElementById('formWaris');

// Interaksi UX: Mengubah label pasangan sesuai jenis kelamin jenazah
jenisKelaminSelect.addEventListener('change', (e) => {
    if (e.target.value === 'P') {
        labelPasangan.textContent = 'Apakah Suami Hidup?';
    } else {
        labelPasangan.textContent = 'Apakah Istri Hidup?';
    }
});

// Event Handler saat tombol Hitung diklik
formWaris.addEventListener('submit', function(e) {
    e.preventDefault(); // Mencegah halaman reload saat submit

    // 1. Pastikan modul Wasm sudah siap digunakan
    if (!hitungWarisCplusPlus) {
        alert("Modul WebAssembly belum siap, harap tunggu sebentar.");
        return;
    }

    // 2. Ambil semua data dari input form HTML
    const hartaKotor = parseFloat(document.getElementById('hartaKotor').value) || 0;
    const hutang     = parseFloat(document.getElementById('hutang').value) || 0;
    const biayaMakam = parseFloat(document.getElementById('biayaMakam').value) || 0;
    const jenisKelaminSelect = document.getElementById('jenisKelaminMayit').value; // 'L' atau 'P'
    const jkChar = jenisKelaminSelect.charCodeAt(0); // Mengubah 'L'/'P' menjadi ASCII code karena C++ menerima char

    const isPasangan = parseInt(document.getElementById('isSuamiIstriHidup').value) || 0;
    const isAyah     = parseInt(document.getElementById('isAyahHidup').value) || 0;
    const isIbu      = parseInt(document.getElementById('isIbuHidup').value) || 0;
    const anakLk     = parseInt(document.getElementById('jumlahAnakLk').value) || 0;
    const anakPr     = parseInt(document.getElementById('jumlahAnakPr').value) || 0;

    console.log("Mengirim data ke WebAssembly C++...");

    // 3. Eksekusi fungsi C++ lewat jembatan Wasm!
    hitungWarisCplusPlus(hartaKotor, hutang, biayaMakam, jkChar, isPasangan, isAyah, isIbu, anakLk, anakPr);
});