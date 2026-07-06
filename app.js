let hitungWarisCplusPlus;
let teksHasilBuffer = "";

if (typeof Module === 'undefined') {
    Module = {};
}
Module.print = function(text) {
    console.log(text);
    teksHasilBuffer += text + "\n"; // Menyimpan hasil output dari C++ ke buffer
};

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

    if (!hitungWarisCplusPlus) {
        alert("Modul WebAssembly belum siap, harap tunggu sebentar.");
        return;
    }

    // 1. Reset tampungan teks lama sebelum menghitung ulang
    teksHasilBuffer = "";

    // 2. Ambil data dari form (Sesuai kode kamu sebelumnya)
    const hartaKotor = parseFloat(document.getElementById('hartaKotor').value) || 0;
    const hutang     = parseFloat(document.getElementById('hutang').value) || 0;
    const biayaMakam = parseFloat(document.getElementById('biayaMakam').value) || 0;
    const jenisKelaminSelect = document.getElementById('jenisKelaminMayit').value;
    const jkChar = jenisKelaminSelect.charCodeAt(0);
    const isPasangan = parseInt(document.getElementById('isSuamiIstriHidup').value) || 0;
    const isAyah     = parseInt(document.getElementById('isAyahHidup').value) || 0;
    const isIbu      = parseInt(document.getElementById('isIbuHidup').value) || 0;
    const anakLk     = parseInt(document.getElementById('jumlahAnakLk').value) || 0;
    const anakPr     = parseInt(document.getElementById('jumlahAnakPr').value) || 0;

    // 3. Eksekusi fungsi C++ (Proses ini akan otomatis memicu Module.print mengisi teksHasilBuffer)
    hitungWarisCplusPlus(hartaKotor, hutang, biayaMakam, jkChar, isPasangan, isAyah, isIbu, anakLk, anakPr);

    // 4. Tampilkan teks yang terkumpul ke elemen GUI HTML
    const elemenOutput = document.getElementById('output-wasm');
    const containerHasil = document.getElementById('section-hasil');

    elemenOutput.textContent = teksHasilBuffer;
    containerHasil.classList.remove('hidden');
    containerHasil.scrollIntoView({ behavior: 'smooth' });
});