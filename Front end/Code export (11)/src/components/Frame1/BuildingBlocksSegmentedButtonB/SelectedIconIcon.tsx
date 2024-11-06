import { memo, SVGProps } from 'react';

const SelectedIconIcon = (props: SVGProps<SVGSVGElement>) => (
  <svg preserveAspectRatio='none' viewBox='0 0 14 10' fill='none' xmlns='http://www.w3.org/2000/svg' {...props}>
    <path
      d='M5.1625 9.5L0.8875 5.225L1.95625 4.15625L5.1625 7.3625L12.0438 0.48125L13.1125 1.55L5.1625 9.5Z'
      fill='#4A4459'
    />
  </svg>
);

const Memo = memo(SelectedIconIcon);
export { Memo as SelectedIconIcon };
