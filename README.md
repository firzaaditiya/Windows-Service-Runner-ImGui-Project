# Windows Application ServiceRunner with Dear ImGui
Sebuah project simple aplikasi desktop yang dibuat menggunakan ImGui v1.86, C++11, GLFW+OpenGL3. Project ini dibuat hanya untuk membantu diri dalam menjalankan suatu service karena malas buka SCM (Service Control Manager) pada Windows, akhirnya nyoba bikin sekalian penasaran sama ImGui yang tampilannya bagus jadi sekalian nyoba aja hehe.

## Tools IDE
Ini adalah IDE dan Tools yang saya gunakan :
- Microsoft Visual Studio 2012 Ultimate
- C++11
- GLFW3
- OpenGL3
- ImGui v.1.86 [ImGui](https://github.com/ocornut/imgui/releases/tag/v1.86)

>`
Alasan kenapa aku make imGui v.1.86 karena Ms VisualStudio 2012 Ultimate hanya support C++11 tidak support C++ di atasnya apalagi C++ Modern, jadi ImGui yang support C++11 adalah versi 1.86, versi diatasnya sudah tidak support C++11, karena di ganti dengan C++ Modern
`
## Preview
Berikut adalah gambaran project ini :

![Ini adalah menu utama dimana kita bisa mengaktifkan dan menonaktifkan service yang sudah kita tambahkan](https://github.com/firzaaditiya/Windows-Service-Runner-ImGui-Project/blob/main/preview-service-runner.png)

pada gambar diatas ini adalah menu utama dimana kita bisa mengaktifkan dan menonaktifkan service yang sudah kita tambahkan.

![Ini adalah menu input dimana kita bisa menambahkan list service agar muncul di menu utama](https://github.com/firzaaditiya/Windows-Service-Runner-ImGui-Project/blob/main/preview-service-runner2.png)

pada gambar ke 2 ini adalah menu input dimana kita bisa menambahkan list service agar muncul di menu utama.

Program masih versi awal mungkin bakal di improve kalo nemu masalah, tapi mungkin nanti bakal ditambah error message sih, tunggu aja

## Licence
Proyek ini dilisensikan di bawah [MIT License](LICENSE).
