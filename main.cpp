#include <windows.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

// STDCALL int Add(int a, int b) { return a + b; }
static constexpr uint8_t program[] = {
  // Prologue
  0x55,             // push ebp
  0x89, 0xE5,       // mov ebp, esp

  // Doing stuff
  0x8B, 0x45, 0x08, // mov eax, DWORD PTR [ebp + 0x8]
  0x8B, 0x55, 0x0C, // mov edx, DWORD PTR [ebp + 0xc]
  0x01, 0xD0,       // add eax, edx

  // Epilogue
  0x5D,             // pop ebp
  0xC3              // ret
};

int main() {
  auto add_function = static_cast<uint8_t*>(
    ::VirtualAlloc(
      NULL, sizeof(program),
      MEM_RESERVE | MEM_COMMIT,
      PAGE_EXECUTE_READWRITE
    )
  );
  if (!add_function) {
    exit(EXIT_FAILURE);
  }

  memcpy(add_function, program, sizeof(program));

  auto f = (long(*)(long, long))add_function;
  const int result = f(2, 3);
  printf("%d\n", result);

  return 0;
}