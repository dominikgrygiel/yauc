guard :shell, all_on_start: true do
  watch(%r{\.(c|h)$}) do
    `make test`
  end
end
