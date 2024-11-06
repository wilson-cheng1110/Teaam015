import { memo, SVGProps } from 'react';

const Icon2 = (props: SVGProps<SVGSVGElement>) => (
  <svg preserveAspectRatio='none' viewBox='0 0 14 14' fill='none' xmlns='http://www.w3.org/2000/svg' {...props}>
    <path d='M6 8H0V6H6V0H8V6H14V8H8V14H6V8Z' fill='#4A4459' />
  </svg>
);

const Memo = memo(Icon2);
export { Memo as Icon2 };
